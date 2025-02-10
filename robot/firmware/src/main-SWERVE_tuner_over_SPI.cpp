/*
spi master. expects a slave to be uploaded to the arduino.
*/

#include <Arduino.h>
#include <SPI.h>
#include <Encoder.h>
#include <Servo.h>
#include <Decodeur.h>
#include <PID_RT.h>
#include <CrcLib.h>
#include <sensors/lin_enco_spoof.hpp>
#include <sensors/gobuilda_rotary_enc.hpp>
#include "communication/enco_peripherals.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print.hpp"
#include "util/looped.hpp"
#include <drives/precision_motor.hpp>
#include <drives/swerve_module.hpp>
#include <controller.hpp>
#include "math/vectors.hpp"
#include "config.hpp"

Decodeur cmd(&Serial);
bool read_mode = true, controller_mode = false;
Timer print_timer(ONE_SECOND / 20);
Timer poll_timer(ONE_SECOND / 40);

dataframe_t df;

LinEncSpoof spoofs[ENCO_NUM] = {
    {df[0], poll_timer}, // right high
    {df[1], poll_timer}, // right low (ITS INVERTED!)
    {df[2], poll_timer}, // left low
    {df[3], poll_timer}, // left high
    {df[4], poll_timer},
    {df[5], poll_timer},
    {df[6], poll_timer},
    {df[7], poll_timer},
};

GobuildaRotaryEncoder goencs[ENCO_NUM] = {
    {spoofs[0], 145.1 * 5, poll_timer},
    {spoofs[1], 145.1 * 5, poll_timer},
    {spoofs[2], 145.1 * 3.55, poll_timer},
    {spoofs[3], 145.1 * 3.55, poll_timer},
    {spoofs[4], 145.1 * 5, poll_timer},
    {spoofs[5], 145.1 * 5, poll_timer},
    {spoofs[6], 145.1 * 5, poll_timer},
    {spoofs[7], 145.1 * 5, poll_timer},
};


Motor motors[NUM_MOTORS] = {
    {CRC_PWM_7}, // Swerve Right B
    {CRC_PWM_1}, // Swerve Right A
    {CRC_PWM_3}, // Swerve Left A
    {CRC_PWM_4}, // Swerve Left B
    {CRC_PWM_5}, // Bras Right
    {CRC_PWM_6}, // Bras Left
    {CRC_PWM_2}, // Poignet
    {CRC_PWM_8}, // À Déterminé - Lanceur
};

PrecisionMotor pmotors[NUM_MOTORS] = {
    {"Swerve Right B", motors[0], goencs[0], MAX_RPM_SWERVE}, // Swerve Right B
    {"Swerve Right A", motors[1], goencs[1], MAX_RPM_SWERVE}, // Swerve Right A
    {"Swerve Left A", motors[2], goencs[2], MAX_RPM_SWERVE},  // Swerve Left A
    {"Swerve Left B", motors[3], goencs[3], MAX_RPM_SWERVE},  // Swerve Left B
    {"Bras Right", motors[4], goencs[4], MAX_RPM_BRAS},       // Bras Right
    {"Bras Left", motors[5], goencs[5], MAX_RPM_BRAS},        // Bras Left
    {"Poignet", motors[6], goencs[6], MAX_RPM_BRAS},          // Poignet
    {"Lanceur", motors[7], goencs[7], MAX_RPM_LANCE},         // Lanceur
};

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco_right(CRC_DIG_1, MAX_PULSE_LEN, 0.0, poll_timer);
PwmRotaryEncoder pwm_enco_left(CRC_DIG_2, MAX_PULSE_LEN, 0.0, poll_timer);

SwerveModule swerve_right(
    pmotors[1],
    pmotors[0],
    pwm_enco_right);

SwerveModule swerve_left(
    pmotors[2],
    pmotors[3],
    pwm_enco_left);

// should always be between 0 and NUM_MOTORS
size_t currently_selected_swerve = 0; // 0=right, 1=left, 2=controller(both, cant tune)

// PID_RT &get_current_pid_to_tune(PrecisionMotor pmotor)
// {
//     return pmotor._mode == PrecisionMotor::Mode::MATCH_ANGLE
//                ? pmotor._pid_angle
//                : pmotor._pid_speed;
// }

SwerveModule &get_swerve(size_t swerve_num)
{
    return swerve_num == 0 ? swerve_right : swerve_left;
}

void execute_commands()
{
    auto &swerve = get_swerve(currently_selected_swerve);

    switch (toupper(cmd.getCommand()))
    {
    case 'T':
    {
        /**
         * donne le target en coordonnes polaires
         */
        auto target_angle = cmd.getArg(0), target_speed = cmd.getArg(1);
        auto target = (swerve_heading){
            .direction = target_angle, .velocity = target_speed};
        swerve.set_target(target);
        Serial.println("Target (@" + String(target.direction) + ")|(s" + String(target.velocity) + ")");
        break;
    }
    case 'C':
    {
        // change the swerve to tune (0=right, 1=left) or enable controller control (2) (TODO: controller mode unimplemented)
        auto selected_swerve = cmd.getArg(0);
        if (selected_swerve == 0 || selected_swerve == 1)
        {
            swerve.enable(false);
            currently_selected_swerve = selected_swerve;
            auto &new_swerve = get_swerve(currently_selected_swerve);
            new_swerve.enable(true);
            controller_mode = false;
        }
        // if (selected_swerve == 2)
        // {
        //     controller_mode = true;
        // }

        break;
    }
    case 'N':
    {
        // reverse the direction of the encoder
        swerve._e.set_inverted(!swerve._e._inverted);
        break;
    }
    case 'K':
    {
        auto Kp = cmd.getArg(0);
        auto Ki = cmd.getArg(1);
        auto Kd = cmd.getArg(2);
        swerve._pid.setK(Kp, Ki, Kd);
        print_pid_vals(swerve._pid);
        break;
    }
    case 'P':
    {
        auto Kp = cmd.getArg(0);
        swerve._pid.setKp(Kp);
        print_pid_vals(swerve._pid);
        break;
    }
    case 'I':
    {
        auto Ki = cmd.getArg(0);
        swerve._pid.setKi(Ki);
        print_pid_vals(swerve._pid);
        break;
    }
    case 'D':
    {
        auto Kd = cmd.getArg(0);
        swerve._pid.setKd(Kd);
        print_pid_vals(swerve._pid);
        break;
    }
    case 'M':
    {
        swerve.enable(!swerve._enabled);
        break;
    }
    case 'R':
    {
        read_mode = !read_mode;
        break;
    }
    case 'X':
    {
        // TODO: generate report
        break;
    }
    }
}

void setup()
{
    Serial.begin(115200);
    CrcLib::Initialize();
    master_enco_spi_init();
    SPI.begin();

    // fixing the same interval for the motors as configuring them.
    for (auto &pmotor : pmotors)
    {
        pmotor._pid_angle.setInterval(poll_timer._delay);
        pmotor._pid_speed.setInterval(poll_timer._delay);
    }
    pmotors_config(pmotors);

    {
        swerve_right.begin();
        swerve_right._pid.setInterval(poll_timer._delay);
        swerve_right._e.set_inverted(true);

        swerve_left.begin();
        swerve_left._pid.setInterval(poll_timer._delay);
        swerve_right._e.set_inverted(true);
    }


    auto &swerve = get_swerve(currently_selected_swerve);
    swerve.enable(true);

    Serial.println("Setup Done");
}

void loop()
{
    auto now = millis();
    print_timer.update(now);
    poll_timer.update(now);
    CrcLib::Update();
    cmd.refresh();
    execute_commands();

    if (poll_timer.is_time())
    {
        retrieve_df(df);
    }

    swerve_right.update();
    swerve_left.update();

    if (read_mode && print_timer.is_time())
    {
        auto &swerve = get_swerve(currently_selected_swerve);

        SPRINT(currently_selected_swerve == 0 ? "[R]" : "[L]");

        SEPARATOR;
        // SPRINT(" ANGLE ");
        // SPRINT("c:");
        SPRINT(String(swerve._e.getLast().rads));
        // SPRINT(" ");
        // SPRINT("t:" + String(swerve._target.direction));
        // SPRINT(" ");

        SEPARATOR;

        auto oprev = swerve.get_oprev_result();
        // SPRINT(" OPREV ");
        SPRINT(oprev.reverse ? "Y" : "N");
        // SPRINT(" ");
        // SPRINT(padLeft(String(oprev.travel), 5, '+'));
        // SPRINT(" ");

        SEPARATOR;

        // SPRINT(" SPEED ");
        // SPRINT("lin:" + padLeft(String(swerve.get_linear_velocity(), 1), 6, '_'));
        // SPRINT(" ");
        // SPRINT("ang:" + padLeft(String(swerve.get_angular_velocity(), 1), 6, '_'));
        // SPRINT(" ");
        // SPRINT("w:" + padLeft(String(swerve.get_wheel_rpm(), 1), 6, '_')); // TODO: re-add when implemented
        SPRINT(" ");
        SPRINT("[A] S:" + padLeft(String(swerve._pma._e.getLast().rpm, 1), 6, '\''));
        SPRINT("  T:" + padLeft(String(swerve._pma._pid_speed.getSetPoint(), 1), 6, '\''));
        SPRINT("  (" + String(swerve._pma._m.get_power()) + ")");
        SEPARATOR;
        SPRINT("[B] S:" + padLeft(String(swerve._pmb._e.getLast().rpm, 1), 6, '\''));
        SPRINT("  T:" + padLeft(String(swerve._pmb._pid_speed.getSetPoint(), 1), 6, '\''));
        SPRINT("  (" + String(swerve._pmb._m.get_power()) + ")");
        SPRINT(" ");

        SEPARATOR;

        // SPRINT(" ");
        // SPRINT("s:" + String(swerve._pid.getSetPoint()) + "rad");
        SPRINT(" ");
        SPRINT("i:" + padLeft(String(swerve._pid.getInput()), 5, '+') + "rad");
        SPRINT(" ");
        SPRINT("o:" + padLeft(String(swerve._pid.getOutput(), 1), 7, '_') + "rpm");
        SPRINT(" ");

        SEPARATOR;

        SPRINT(" K ");
        SPRINT(String(swerve._pid.getKp(), 5));
        SPRINT(" ");
        SPRINT(String(swerve._pid.getKi(), 5));
        SPRINT(" ");
        SPRINT(String(swerve._pid.getKd(), 5));
        SPRINT(" ");

        SEPARATOR;

        Serial.println();
    }
}

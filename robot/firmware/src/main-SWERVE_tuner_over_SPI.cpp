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
#include "communication/enco_peripherals_uno.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print_crc_extras.hpp"
#include "util/looped.hpp"
#include <drives/precision_motor.hpp>
#include <drives/swerve_module.hpp>
#include "math/vectors.hpp"
#include "config.hpp"
#include "communication/experimental_controller.hpp"


Decodeur cmd(&Serial);
ExController ctlr;
bool read_mode = true, controller_mode;
Timer print_timer(ONE_SECOND / 20);
Timer poll_timer(ONE_SECOND / 20);
Timer swerve_timer(ONE_SECOND / 20);


dataframe_t df;

LinEncSpoof spoofs[ENCO_NUM] = {
    {df[0], poll_timer}, // swerve r a
    {df[1], poll_timer}, // swerv r b
    {df[2], poll_timer}, // arm left
    {df[3], poll_timer}, // launcher
    {df[4], poll_timer}, // wrist
    {df[5], poll_timer}, // arm right
    {df[6], poll_timer}, // swerve l a
    {df[7], poll_timer}, // swerve l b
};

GobuildaRotaryEncoder goencs[ENCO_NUM] = {
    {spoofs[0], 145.1 * 5, poll_timer},
    {spoofs[1], 145.1 * 5, poll_timer},
    {spoofs[2], 145.1 * 5, poll_timer},
    {spoofs[3], 145.1 * 5, poll_timer},
    {spoofs[4], 145.1 * 5, poll_timer},
    {spoofs[5], 145.1 * 5, poll_timer},
    {spoofs[6], 145.1 * 5, poll_timer},
    {spoofs[7], 145.1 * 5, poll_timer},
};

Motor motors[NUM_MOTORS] = {
    {CRC_PWM_4},  // Swerve Right A
    {CRC_PWM_3},  // Swerve Right B
    {CRC_PWM_11}, // Swerve Left A
    {CRC_PWM_12}, // Swerve Left B
    {CRC_PWM_5},  // Bras Right
    {CRC_PWM_2},  // Bras Left
    {CRC_PWM_6},  // Poignet
    {CRC_PWM_1},  // Lanceur
};

PrecisionMotor pmotors[NUM_MOTORS] = {
    {"Swerve Right A", motors[0], goencs[0], MAX_RPM_SWERVE},
    {"Swerve Right B", motors[1], goencs[1], MAX_RPM_SWERVE},
    {"Swerve Left A", motors[2], goencs[6], MAX_RPM_SWERVE},
    {"Swerve Left B", motors[3], goencs[7], MAX_RPM_SWERVE},
    {"Bras Right", motors[4], goencs[5], MAX_RPM_BRAS},
    {"Bras Left", motors[5], goencs[2], MAX_RPM_BRAS},
    {"Poignet", motors[6], goencs[4], MAX_RPM_BRAS},
    {"Lanceur", motors[7], goencs[3], MAX_RPM_LANCE},
};

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco_left(CRC_DIG_1, MAX_PULSE_LEN, -1.07, poll_timer);
PwmRotaryEncoder pwm_enco_right(CRC_DIG_2, MAX_PULSE_LEN, -2.10, poll_timer);

SwerveModule swerve_right(pmotors[I_RAS], pmotors[I_RBS], pwm_enco_right);
SwerveModule swerve_left(pmotors[I_LAS], pmotors[I_LBS], pwm_enco_left);

SwerveDrive sd(swerve_right, swerve_left);

size_t currently_selected_swerve = 0; // 0=left, 1=right, 2=controller(both, cant tune)

// PID_RT &get_current_pid_to_tune(PrecisionMotor pmotor)
// {
//     return pmotor._mode == PrecisionMotor::Mode::MATCH_ANGLE
//                ? pmotor._pid_angle
//                : pmotor._pid_speed;
// }

SwerveModule &get_swerve(size_t swerve_num)
{
    return swerve_num == 0 ? swerve_left :swerve_right ;
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
    ctlr.begin();

    // fixing the same interval for the motors as configuring them.
    for (auto &pmotor : pmotors)
    {
        pmotor._pid_angle.setInterval(poll_timer._delay);
        pmotor._pid_speed.setInterval(poll_timer._delay);
    }
    pmotors_config(pmotors);

    {
        swerve_right.begin();
        swerve_left.begin();

        swerve_right._pid.setInterval(swerve_timer._delay);
        swerve_right._e.set_inverted(true);
        swerve_right._pid.setK(150, 0, 100);

        swerve_left._pid.setInterval(swerve_timer._delay);
        swerve_left._e.set_inverted(true);
        swerve_left._pid.setK(150, 0, 100);
    }
    swerve_config(sd);

    alduino_reset();
    auto &swerve = get_swerve(currently_selected_swerve);
    swerve.enable(true);

    Serial.println("Setup Done");


}

void loop()
{
    auto now = millis();
    print_timer.update(now);
    poll_timer.update(now);
    swerve_timer.update(now);
    CrcLib::Update();
    cmd.refresh();
    ctlr.update();
    execute_commands();

        if (ctlr._raw.select)
    // if (exctrl._raw.start)
    {
        alduino_reset();
    }
    static bool started = false;
    if (ctlr._raw.start) {
        Serial.println("out!");
        started = true;
    }

    if (!started) {
        return;
    }

    if (poll_timer.is_time())
    {
        retrieve_df(df);
    }

    swerve_right.update();
    swerve_left.update();

    if (read_mode && print_timer.is_time())
    {
        auto &swerve = get_swerve(currently_selected_swerve);

        print_battery();

        SPRINT(currently_selected_swerve == 1 ? "[R]" : "[L]");
        SEPARATOR;

        SPRINT(String(swerve._e.getLast().rads));

        SEPARATOR;
        auto oprev = swerve.get_oprev_result();
        SPRINT(oprev.reverse ? "Y" : "N");
        SEPARATOR;

        SPRINT(swerve.get_angular_velocity());
        SPRINT(" * ");
        SPRINT(swerve._target.velocity * swerve._mtwr);
        SEPARATOR;

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

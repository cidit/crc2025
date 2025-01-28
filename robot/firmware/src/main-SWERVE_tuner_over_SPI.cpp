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
#include <drives/precision_motor2.hpp>
#include <drives/swerve_module2.hpp>
#include <controller.hpp>
#include "math/vectors.hpp"

int32_t enco_values[ENCO_NUM];

Decodeur cmd(&Serial);
bool read_mode = true, controller_mode = false;
Timer print_timer(ONE_SECOND / 40);
Timer poll_timer(ONE_SECOND / 10000);

const size_t NUM_MOTORS = 4;

int32_t df[ENCO_NUM];

LinEncSpoof spoofs[ENCO_NUM] = {
    LinEncSpoof(df[0], poll_timer), // right high
    LinEncSpoof(df[1], poll_timer), // right low (ITS INVERTED!)
    LinEncSpoof(df[2], poll_timer), // left low
    LinEncSpoof(df[3], poll_timer), // left high
    LinEncSpoof(df[4], poll_timer),
    LinEncSpoof(df[5], poll_timer),
    LinEncSpoof(df[6], poll_timer),
    LinEncSpoof(df[7], poll_timer),
};

GobuildaRotaryEncoder goencs[ENCO_NUM] = {
    GobuildaRotaryEncoder(spoofs[0], 145.1 * 2.5, poll_timer),
    GobuildaRotaryEncoder(spoofs[1], 145.1 * 2.5, poll_timer),
    GobuildaRotaryEncoder(spoofs[2], 145.1 * 2.5, poll_timer, true),
    GobuildaRotaryEncoder(spoofs[3], 145.1 * 2.5, poll_timer),
    GobuildaRotaryEncoder(spoofs[4], 145.1 * 2.5, poll_timer),
    GobuildaRotaryEncoder(spoofs[5], 145.1 * 2.5, poll_timer),
    GobuildaRotaryEncoder(spoofs[6], 145.1 * 2.5, poll_timer),
    GobuildaRotaryEncoder(spoofs[7], 145.1 * 2.5, poll_timer),
};

Motor motors[NUM_MOTORS] = {
    Motor(CRC_PWM_4),
    Motor(CRC_PWM_3),
    Motor(CRC_PWM_1),
    Motor(CRC_PWM_7),
};

PrecisionMotor pmotors[NUM_MOTORS] = {
    PrecisionMotor(motors[0], goencs[0], 400.),
    PrecisionMotor(motors[1], goencs[1], 400.),
    PrecisionMotor(motors[2], goencs[2], 400.),
    PrecisionMotor(motors[3], goencs[3], 400.),
};

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco_right(CRC_DIG_1, MAX_PULSE_LEN, 0.0, poll_timer); // TODO: get right pin
PwmRotaryEncoder pwm_enco_left(CRC_DIG_2, MAX_PULSE_LEN, 0.0, poll_timer);

SwerveModule swerve_right(
    pmotors[0],
    pmotors[1],
    pwm_enco_right);

SwerveModule swerve_left(
    pmotors[3],
    pmotors[2],
    pwm_enco_left);

// should always be between 0 and NUM_MOTORS
size_t currently_selected_swerve = 0; // 0=right, 1=left, 2=controller(both, cant tune)

void update_df()
{
    // TODO: for some reason, the values seem to be multiplied by 256
    retrieve_df(df);
    // here, we sorta patch the multiplied by 256 problem
    for (int i = 0; i < ENCO_NUM; i++)
    {
        df[i] /= 256;
    }
}

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
    auto swerve = get_swerve(currently_selected_swerve);

    switch (toupper(cmd.getCommand()))
    {
    case 'X':
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
            currently_selected_swerve = selected_swerve;
            controller_mode = false;
        }
        if (selected_swerve == 2)
        {
            controller_mode = true;
        }

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
    }
}

void setup()
{
    Serial.begin(115200);

    CrcLib::Initialize();

    master_enco_spi_init();
    SPI.begin();

    for (auto &pmotor : pmotors)
    {
        pmotor._pid_angle.setInterval(poll_timer._delay);
        pmotor._pid_speed.setInterval(poll_timer._delay);
        pmotor.begin();
        pmotor.enable(true);
    }

    {
        swerve_right.begin();
        swerve_right._pma._pid_speed.setK(0.60000, 0.00001, 0.12000);
        swerve_right._pmb._pid_speed.setK(0.60000, 0.00001, 0.15500);
        swerve_right.enable(true);

        swerve_left.begin();
        swerve_left._pma._pid_speed.setK(0.60000, 0.00001, 0.12000);
        swerve_left._pmb._pid_speed.setK(0.60000, 0.00001, 0.15500);
        swerve_left.enable(true);
    }

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
        update_df();
    }

    swerve_right.update();
    swerve_left.update();

    if (read_mode && print_timer.is_time())
    {
        auto &swerve = get_swerve(currently_selected_swerve);

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
        SPRINT("  {" + String(swerve._pma._m.get_power()) + "}");
        SEPARATOR;
        SPRINT("[B] S:" + padLeft(String(swerve._pmb._e.getLast().rpm, 1), 6, '\''));
        SPRINT("  T:" + padLeft(String(swerve._pmb._pid_speed.getSetPoint(), 1), 6, '\''));
        SPRINT("  {" + String(swerve._pmb._m.get_power()) + "}");
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

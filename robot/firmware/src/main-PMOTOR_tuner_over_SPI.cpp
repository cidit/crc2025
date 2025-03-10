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
#include <drives/precision_motor.hpp>
#include "util/looped.hpp"
#include "math/vectors.hpp"
#include "config.hpp"

Decodeur cmd(&Serial);
bool read_mode = true;
Timer print_timer(ONE_SECOND / 20);
Timer poll_timer(ONE_SECOND / 20);

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
    {CRC_PWM_10}, // Swerve Right A
    {CRC_PWM_9},  // Swerve Right B
    {CRC_PWM_11}, // Swerve Left A
    {CRC_PWM_12}, // Swerve Left B
    {CRC_PWM_5},  // Bras Right
    {CRC_PWM_8},  // Bras Left
    {CRC_PWM_6},  // Poignet
    {CRC_PWM_7},  // Lanceur
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

// should always be between 0 and NUM_MOTORS
size_t currently_selected_pmotor_idx = 0;

void configure_motors()
{
    for (auto &pmotor : pmotors)
    {
        pmotor._pid_angle.setInterval(poll_timer._delay);
        pmotor._pid_speed.setInterval(poll_timer._delay);
        pmotor.begin();
    }
}

PID_RT &get_current_pid_to_tune(PrecisionMotor &pmotor)
{
    return pmotor._mode == PrecisionMotor::Mode::MATCH_ANGLE
               ? pmotor._pid_angle
               : pmotor._pid_speed;
}

PrecisionMotor &get_pmotor(size_t pmotor_idx)
{
    return pmotors[pmotor_idx];
}

void execute_commands()
{
    auto &pmotor = get_pmotor(currently_selected_pmotor_idx);
    auto &tuning_pid = get_current_pid_to_tune(pmotor);

    switch (toupper(cmd.getCommand()))
    {
    case '\'':
    {
        alduino_reset();
        break;
    }
    case 'Z':
    {
        // change which motor is selected. disable the currently selected motor and enables the selected one
        auto selected_idx = cmd.getArg(0);
        if (selected_idx >= 0 && selected_idx < NUM_MOTORS)
        {
            auto &old_pm = get_pmotor(currently_selected_pmotor_idx);
            old_pm.enable(false);
            currently_selected_pmotor_idx = selected_idx;
            auto &new_pm = get_pmotor(currently_selected_pmotor_idx);
            new_pm.enable(true);
        }
        break;
    }
    case 'S':
    {
        // change speed
        auto targetRPM = cmd.getArg(0);
        pmotor.set_target_rpm(targetRPM);
        Serial.println("Target RPM: " + String(targetRPM));
        break;
    }
    case 'A':
    {
        // change angle
        auto target_angle = cmd.getArg(0);
        pmotor.set_target_angle(target_angle);
        Serial.println("Target angle: " + String(target_angle));
        break;
    }

    case 'Q':
    {
        // toggle current motor
        pmotor.enable(!pmotor._enabled);
        break;
    }
    case 'W':
    {
        pmotor._m.set_power_ratio(cmd.getArg(0));
        break;
    }

    // pid shit
    case 'K':
    {
        // set all pid constants
        auto Kp = cmd.getArg(0);
        auto Ki = cmd.getArg(1);
        auto Kd = cmd.getArg(2);
        tuning_pid.setK(Kp, Ki, Kd);
        print_pid_vals(tuning_pid);
        break;
    }
    case 'P':
    {
        // set proportionnal pid constant
        auto Kp = cmd.getArg(0);
        tuning_pid.setKp(Kp);
        print_pid_vals(tuning_pid);
        break;
    }
    case 'I':
    {
        // set integral pid constant
        auto Ki = cmd.getArg(0);
        tuning_pid.setKi(Ki);
        print_pid_vals(tuning_pid);
        break;
    }
    case 'D':
    {
        // set derivative pid constant
        auto Kd = cmd.getArg(0);
        tuning_pid.setKd(Kd);
        print_pid_vals(tuning_pid);
        break;
    }

    case 'R':
    {
        // enable/disable printing of values
        read_mode = !read_mode;
        break;
    }

    // toggle encoder and motor directions
    case 'N':
    {
        // toggle reverse state of encoder
        pmotor._e._is_inverted = !pmotor._e._is_inverted;
        break;
    }
    case 'M':
    {
        // toggle reverse state of motor
        pmotor._m.set_inverted(!pmotor._m._is_inverted);
        break;
    }

    case 'X':
    {
        // generate a report
        for (size_t i = 0; i < NUM_MOTORS; i++)
        {
            auto &pm = get_pmotor(i);
            Serial.println("/* PMOTOR #[" + String(i) + "] " + pm.display_name() + " config */ {");
            Serial.println("\tauto &pm = pmotors[" + String(i) + "];");
            Serial.println("\tpm._e.set_inverted(" + String(pm._e._is_inverted ? "true" : "false") + ");");
            Serial.println("\tpm._m.set_inverted(" + String(pm._m._is_inverted ? "true" : "false") + ");");
            Serial.println("\tpm._pid_angle.setK(" +
                           String(pm._pid_angle.getKp(), 5) +
                           ", " +
                           String(pm._pid_angle.getKi(), 5) +
                           ", " +
                           String(pm._pid_angle.getKd(), 5) +
                           ");");
            Serial.println("\tpm._pid_speed.setK(" +
                           String(pm._pid_speed.getKp(), 5) +
                           ", " +
                           String(pm._pid_speed.getKi(), 5) +
                           ", " +
                           String(pm._pid_speed.getKd(), 5) +
                           ");");
            Serial.println("};");
        }
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

    pmotors_config(pmotors);
    configure_motors();

    auto &pm = get_pmotor(currently_selected_pmotor_idx);
    pm.enable(true);

    alduino_reset();
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

    if (cmd.isAvailable())
    {
        if (cmd.getCommand() == '\'')
        {
            alduino_reset();
        }
    }

    if (poll_timer.is_time())
    {
        retrieve_df(df);
    }

    for (auto &pmotor : pmotors)
    {
        pmotor.update();
    }

    if (read_mode && print_timer.is_time())
    {
        auto &pmotor = get_pmotor(currently_selected_pmotor_idx);
        auto &tuning_pid = get_current_pid_to_tune(pmotor);

        SPRINT("[");
        SPRINT(pmotor._mode == PrecisionMotor::Mode::MATCH_ANGLE ? 'A' : 'S');
        SPRINT("#");
        SPRINT(currently_selected_pmotor_idx);
        SPRINT("|R");
        SPRINT(" E");                               // encoder reveresd?
        SPRINT(pmotor._e._is_inverted ? "Y" : "N"); // yes reversed, no reversed
        SPRINT(" M");                               // motor reversed?
        SPRINT(pmotor._m._is_inverted ? "Y" : "N"); // yes reversed, no reversed
        SPRINT("]");
        SEPARATOR;

        SPRINT("speed:" + padRight(String(pmotor._e.getLast().rpm), 7, '\''));
        SEPARATOR;
        SPRINT("angle:" + String(pmotor._e.getLast().rads, 2));
        SEPARATOR;

        SPRINT("enco:" + padRight(String(pmotor._e._internal_read()), 4, '\''));
        SEPARATOR;

        SPRINT("[ ");
        SPRINT("s:" + padRight(String(tuning_pid.getSetPoint()), 7, '\''));
        SPRINT(" ");
        SPRINT("i:" + padRight(String(tuning_pid.getInput()), 7, '\''));
        SPRINT(" ");
        SPRINT("o:" + padRight(String(tuning_pid.getOutput()), 7, '\''));
        SPRINT(" ]");
        SEPARATOR;

        SPRINT("P%:" + String(pmotor._m._last_power, 4));
        SEPARATOR;

        SPRINT("[ K ");
        SPRINT(padRight(String(tuning_pid.getKp(), 5), 7, '\''));
        SPRINT(" ");
        SPRINT(padRight(String(tuning_pid.getKi(), 5), 7, '\''));
        SPRINT(" ");
        SPRINT(padRight(String(tuning_pid.getKd(), 5), 7, '\''));
        SPRINT(" ]");
        SEPARATOR;

        hexdump_df(df);
        Serial.println();
    }
}
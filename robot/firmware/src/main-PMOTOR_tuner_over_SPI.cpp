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
#include <drives/precision_motor.hpp>
#include <controller.hpp>
#include "util/looped.hpp"
#include "math/vectors.hpp"

int32_t enco_values[ENCO_NUM];

Decodeur cmd(&Serial);
bool read_mode = true;
Timer print_timer(ONE_SECOND / 100);
Timer poll_timer(ONE_SECOND / 40);

const size_t NUM_MOTORS = 4;

dataframe_t df;

LinEncSpoof spoofs[ENCO_NUM] = {
    {df[0], poll_timer},
    {df[1], poll_timer},
    {df[2], poll_timer},
    {df[3], poll_timer},
    {df[4], poll_timer},
    {df[5], poll_timer},
    {df[6], poll_timer},
    {df[7], poll_timer},
};

GobuildaRotaryEncoder goencs[ENCO_NUM] = {
    {spoofs[0], 145.1 * 2.5, poll_timer, true},
    {spoofs[1], 145.1 * 2.5, poll_timer},
    {spoofs[2], 145.1 * 2.5, poll_timer, true},
    {spoofs[3], 145.1 * 2.5, poll_timer},
    {spoofs[4], 145.1 * 2.5, poll_timer},
    {spoofs[5], 145.1 * 2.5, poll_timer},
    {spoofs[6], 145.1 * 2.5, poll_timer},
    {spoofs[7], 145.1 * 2.5, poll_timer},
};

Motor motors[NUM_MOTORS] = {
    {CRC_PWM_4, true},
    {CRC_PWM_3},
    {CRC_PWM_1},
    {CRC_PWM_7},
};

PrecisionMotor pmotors[NUM_MOTORS] = {
    {motors[0], goencs[0], 400.},
    {motors[1], goencs[1], 400.},
    {motors[2], goencs[2], 400.},
    {motors[3], goencs[3], 400.},
};

// should always be between 0 and NUM_MOTORS
size_t currently_selected_pmotor_idx = 0;

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

    Serial.println("Setup Done");
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
    case 'C':
    {
        // change which motor is selected
        auto selected_idx = cmd.getArg(0);
        if (selected_idx >= 0 && selected_idx < NUM_MOTORS)
        {
            currently_selected_pmotor_idx = selected_idx;
        }
        break;
    }
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
    case 'M':
    {
        // enable/disable current motor
        pmotor.enable(!pmotor._enabled);
        break;
    }
    case 'R':
    {
        // enable/disable printing of values
        read_mode = !read_mode;
        break;
    }
    }
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

        Serial.println();

        hexdump_df(df);
    }
}
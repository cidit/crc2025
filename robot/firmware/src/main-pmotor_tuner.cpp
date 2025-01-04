
#include <CrcLib.h>
#include <Encoder.h>
#include <drives/precision_motor2.hpp>
#include <controller.hpp>
#include "util/looped.hpp"
#include "math/vectors.hpp"
#include <Servo.h>
#include <Decodeur.h>
#include <PID_RT.h>
using math::cartesian::Vec2D;

#define SPRINT(things) Serial.print(things)
#define SPACER Serial.print("    ")

Decodeur cmd(&Serial);
bool read_mode = true;
Timer print_timer(ONE_SECOND / 10);

Motor motor(CRC_PWM_1);
Encoder enco(CRC_ENCO_B, CRC_DIG_3);
PrecisionMotor pmotor(motor, enco, 145.1 * 2.5, 1150);

void setup()
{
    Serial.begin(115200);

    CrcLib::Initialize();

    pmotor.begin();
    pmotor.enable(true);

    Serial.println("Setup Done");
}

PID_RT &get_current_pid_to_tune()
{
    return pmotor._mode == PrecisionMotor::Mode::MATCH_ANGLE
               ? pmotor._pid_angle
               : pmotor._pid_speed;
}

void print_pid_vals()
{
    auto &tuning_pid = get_current_pid_to_tune();
    Serial.println("Kp: " +
                   String(tuning_pid.getKp(), 5) +
                   " Ki: " +
                   String(tuning_pid.getKi(), 5) +
                   " Kd: " +
                   String(tuning_pid.getKd(), 5));
}

void execute_commands()
{
    auto &tuning_pid = get_current_pid_to_tune();

    switch (toupper(cmd.getCommand()))
    {
    case 'S':
    {
        auto targetRPM = cmd.getArg(0);
        pmotor.set_target_rpm(targetRPM);
        Serial.println("Target RPM: " + String(targetRPM));
        break;
    }

    case 'A':
    {
        auto target_angle = cmd.getArg(0);
        pmotor.set_target_angle(target_angle);
        Serial.println("Target angle: " + String(target_angle));
        break;
    }
    case 'K':
    {
        auto Kp = cmd.getArg(0);
        auto Ki = cmd.getArg(1);
        auto Kd = cmd.getArg(2);
        tuning_pid.setK(Kp, Ki, Kd);
        print_pid_vals();
        break;
    }
    case 'P':
    {
        auto Kp = cmd.getArg(0);
        tuning_pid.setKp(Kp);
        print_pid_vals();
        break;
    }
    case 'I':
    {
        auto Ki = cmd.getArg(0);
        tuning_pid.setKi(Ki);
        print_pid_vals();
        break;
    }
    case 'D':
    {
        auto Kd = cmd.getArg(0);
        tuning_pid.setKd(Kd);
        print_pid_vals();
        break;
    }
    case 'M':
    {
        pmotor.enable(!pmotor._enabled);
        break;
    }
    case 'R':
    {
        read_mode = !read_mode;
        break;
    }
    }
}

// https://github.com/ElvisKremmen/Arduino-Code-Fragments/blob/main/Numeric%20string%20left-pad%20function
String padLeft(String inString, uint16_t newLen)
{ // Pad a numeric string with spaces for output
    while (inString.length() < newLen)
    {
        inString = String(" ") + inString;
    };
    return inString;
}

void loop()
{
    auto now = millis();
    CrcLib::Update();
    cmd.refresh();
    execute_commands();
    pmotor.update();

    if (read_mode && print_timer.is_time(now))
    {
        auto &tuning_pid = get_current_pid_to_tune();

        if (pmotor._mode == PrecisionMotor::Mode::MATCH_SPEED)
        {
            SPRINT("speed:" + padLeft(String(pmotor.get_current_rpm()), 7));
        }
        else
        {
            SPRINT("angle:" + String(pmotor.get_current_angle(), 2));
        }
        SPACER;
        SPRINT("enco:" + padLeft(String(pmotor._delta_ticks()), 4));
        SPACER;

        SPRINT("[ ");
        SPRINT("s:" + padLeft(String(tuning_pid.getSetPoint()), 7));
        SPRINT(" ");
        SPRINT("i:" + padLeft(String(tuning_pid.getInput()), 7));
        SPRINT(" ");
        SPRINT("o:" + padLeft(String(tuning_pid.getOutput()), 7));
        SPRINT(" ]");
        SPACER;

        SPRINT("P%:" + padLeft(String(pmotor._m._last_power), 7));
        SPACER;

        SPRINT("[ K ");
        SPRINT(padLeft(String(tuning_pid.getKp(), 5), 7));
        SPRINT(" ");
        SPRINT(padLeft(String(tuning_pid.getKi(), 5), 7));
        SPRINT(" ");
        SPRINT(padLeft(String(tuning_pid.getKd(), 5), 7));
        SPRINT(" ]");
        SPACER;

        Serial.println();
    }
}
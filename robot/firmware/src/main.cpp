
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

drives::Motor motor(CRC_PWM_1);
Encoder enco(CRC_ENCO_B, CRC_DIG_3);
PID_RT pid_speed, pid_angle;
// TODO: more copy problems...
drives::PrecisionMotor2 pmotor(motor, enco, pid_speed, pid_angle, 145.1 * 2.5);

void setup()
{
    Serial.begin(115200);

    CrcLib::Initialize();

    pmotor._pid_speed.setInterval(20);
    pmotor._pid_speed.setK(0, 0, 0);
    pmotor._pid_speed.setPoint(0);
    pmotor._pid_speed.setPropOnError();
    pmotor._pid_speed.setReverse(true);

    pmotor.begin();
    pmotor.enable(true);

    Serial.println("Setup Done");
}

void print_pid_vals()
{
    Serial.println("Kp: " +
                   String(pmotor._pid_speed.getKp(), 5) +
                   " Ki: " +
                   String(pmotor._pid_speed.getKi(), 5) +
                   " Kd: " +
                   String(pmotor._pid_speed.getKd(), 5));
}

void execute_commands()
{
    switch (toupper(cmd.getCommand()))
    {
    case 'T':
    {
        auto targetRPM = cmd.getArg(0);
        pmotor.set_target_rpm(targetRPM);
        Serial.println("Target RPM: " + String(targetRPM));
        break;
    }
    case 'K':
    {
        auto Kp = cmd.getArg(0);
        auto Ki = cmd.getArg(1);
        auto Kd = cmd.getArg(2);
        pmotor._pid_speed.setK(Kp, Ki, Kd);
        print_pid_vals();
        break;
    }
    case 'P':
    {
        auto Kp = cmd.getArg(0);
        pmotor._pid_speed.setKp(Kp);
        print_pid_vals();
        break;
    }
    case 'I':
    {
        auto Ki = cmd.getArg(0);
        pmotor._pid_speed.setKi(Ki);
        print_pid_vals();
        break;
    }
    case 'D':
    {
        auto Kd = cmd.getArg(0);
        pmotor._pid_speed.setKd(Kd);
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
    pmotor.loop();

    if (read_mode && print_timer.is_time(now))
    {
        SPRINT("speed:" + padLeft(String(pmotor.get_current_rpm()), 7));
        SPACER;
        SPRINT("enco:" + padLeft(String(pmotor._delta_ticks()), 4));
        SPACER;

        SPRINT("[ ");
        SPRINT("s:" + padLeft(String(pmotor._pid_speed.getSetPoint()), 7));
        SPRINT(" ");
        SPRINT("i:" + padLeft(String(pmotor._pid_speed.getInput()), 7));
        SPRINT(" ");
        SPRINT("o:" + padLeft(String(pmotor._pid_speed.getOutput()), 7));
        SPRINT(" ]");
        SPACER;

        SPRINT("P%:" + padLeft(String(pmotor._m._last_power), 7));
        SPACER;

        SPRINT("[ K ");
        SPRINT(padLeft(String(pmotor._pid_speed.getKp(), 5), 7));
        SPRINT(" ");
        SPRINT(padLeft(String(pmotor._pid_speed.getKi(), 5), 7));
        SPRINT(" ");
        SPRINT(padLeft(String(pmotor._pid_speed.getKd(), 5), 7));
        SPRINT(" ]");
        SPACER;

        Serial.println();
    }
}
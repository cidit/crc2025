
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

#define SP(things) Serial.print(things)

Decodeur cmd(&Serial);
bool read_mode = false;
Timer print_timer(ONE_SECOND / 10);

drives::Motor motor(CRC_PWM_1);
Encoder enco(CRC_ENCO_B, CRC_DIG_3);
PID_RT pid;
drives::PrecisionMotor2 pmotor(motor, enco, pid, 145.1);

void setup()
{
    Serial.begin(115200);

    CrcLib::Initialize();

    pmotor._pid.setInterval(20);
    pmotor._pid.setK(0, 0, 0);
    pmotor._pid.setPoint(0);
    pmotor._pid.setPropOnError();

    read_mode = true;
    pmotor.begin();

    Serial.println("Setup Done");



    /** TIENS, GUILLAUME. JOUE A PARTIR DE ICI. */
    pmotor._pid.setK(0, 0, 0);
    pmotor._pid.setPoint(0);
    pmotor.enable(true);
}

void print_pid_vals()
{
    Serial.println("Kp: " + 
        String(pmotor._pid.getKp(), 5) + 
        " Ki: " + 
        String(pmotor._pid.getKi(), 5) + 
        " Kd: " + 
        String(pmotor._pid.getKd(), 5)
        );
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
        pmotor._pid.setK(Kp, Ki, Kd);
        print_pid_vals();
        break;
    }
    case 'P':
    {
        auto Kp = cmd.getArg(0);
        pmotor._pid.setKp(Kp);
        print_pid_vals();
        break;
    }
    case 'I':
    {
        auto Ki = cmd.getArg(0);
        pmotor._pid.setKi(Ki);
        print_pid_vals();
        break;
    }
    case 'D':
    {
        auto Kd = cmd.getArg(0);
        pmotor._pid.setKd(Kd);
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

void loop()
{
    auto now = millis();
    CrcLib::Update();
    cmd.refresh();
    // execute_commands();
    pmotor.loop();

    if (read_mode && print_timer.is_time(now))
    {
        Serial.print("speed: " + String(pmotor.get_current_rpm()));
        Serial.print("\t");
        Serial.print("enco: " + String(pmotor._e.read()));
        Serial.print("\t");
        Serial.print("s: " + String(pmotor._pid.getSetPoint()));
        Serial.print("\t");
        Serial.print("i: " + String(pmotor._pid.getInput()));
        Serial.print("\t");
        Serial.print("o: " + String(pmotor._pid.getOutput()));
        Serial.println();
    }
}
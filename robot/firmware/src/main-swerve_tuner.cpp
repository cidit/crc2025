
#include <CrcLib.h>
#include <Encoder.h>
#include <drives/precision_motor2.hpp>
#include <drives/swerve_module2.hpp>
#include <controller.hpp>
#include "util/looped.hpp"
#include "math/vectors.hpp"
#include <Servo.h>
#include <Decodeur.h>
#include <PID_RT.h>
#include "util/print.hpp"

Decodeur cmd(&Serial);
bool read_mode = true, controller_mode = true;
Timer print_timer(ONE_SECOND / 10), polling_timer(ONE_SECOND / 50);

Motor motor1(CRC_PWM_1);
Encoder enco1(CRC_ENCO_B, CRC_DIG_3);
GobuildaRotaryEncoder goenco1(enco1, 145.1 * 2.5, polling_timer);
PrecisionMotor pmotor1(motor1, goenco1, 400);

Motor motor2(CRC_PWM_7);
Encoder enco2(CRC_I2C_SCL, CRC_DIG_5);
GobuildaRotaryEncoder goenco2(enco2, 145.1 * 2.5, polling_timer);
PrecisionMotor pmotor2(motor2, goenco2, 400);

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco(CRC_DIG_1, MAX_PULSE_LEN, polling_timer);

SwerveModule swerve1(pmotor1, pmotor2, pwm_enco);

void setup()
{
    Serial.begin(115200);
    CrcLib::Initialize();

    swerve1.begin();
    swerve1.enable(true);

    Serial.println("Setup Done");
}

void execute_commands()
{

    switch (toupper(cmd.getCommand()))
    {
    case 'Z':
    {
        /**
         * donne le target en coordonnees vectorielles
         */
        auto target_x_rpm = cmd.getArg(0), target_y_rpm = cmd.getArg(1);
        swerve1.set_target(Vec2D(target_x_rpm, target_y_rpm));
        Serial.println("Target RPM: (vx:" + String(target_x_rpm) + ")|(vy:" + String(target_y_rpm) + ")");
        break;
    }
    case 'X':
    {
        /**
         * donne le target en coordonnes polaires
         */
        auto target_angle = cmd.getArg(0), target_speed = cmd.getArg(1);
        swerve1.set_target(Vec2D::from_polar(target_angle, target_speed));
        Serial.println("Target (@" + String(target_angle) + ")|(s" + String(target_speed) + ")");
        break;
    }
    case 'K':
    {
        auto Kp = cmd.getArg(0);
        auto Ki = cmd.getArg(1);
        auto Kd = cmd.getArg(2);
        swerve1._pid.setK(Kp, Ki, Kd);
        print_pid_vals(swerve1._pid);
        break;
    }
    case 'P':
    {
        auto Kp = cmd.getArg(0);
        swerve1._pid.setKp(Kp);
        print_pid_vals(swerve1._pid);
        break;
    }
    case 'I':
    {
        auto Ki = cmd.getArg(0);
        swerve1._pid.setKi(Ki);
        print_pid_vals(swerve1._pid);
        break;
    }
    case 'D':
    {
        auto Kd = cmd.getArg(0);
        swerve1._pid.setKd(Kd);
        print_pid_vals(swerve1._pid);
        break;
    }
    case 'M':
    {
        swerve1.enable(!swerve1._enabled);
        break;
    }
    case 'R':
    {
        read_mode = !read_mode;
        break;
    }
    case 'C':
    {
        controller_mode = !controller_mode;
        break;
    }
    }
}

struct controller
{
    Vec2D right, left;
};
controller read_controller()
{
    return (controller){
        .right = Vec2D(
            double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X)) / HALF_PWM_OUTPUT,
            double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)) / HALF_PWM_OUTPUT).normalize(),
        .left = Vec2D(
            double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X)) / HALF_PWM_OUTPUT,
            double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y)) / HALF_PWM_OUTPUT).normalize()};
}

void apply_controller_input()
{
    controller c = !CrcLib::IsCommValid()
                       ? (controller){.right = Vec2D(0, 0), .left = Vec2D(0, 0)}
                       : read_controller();
    swerve1.set_target(Vec2D(c.left.x() * 500, c.left.y() * 500));
}

void loop()
{
    auto now = millis();
    print_timer.update(now);
    polling_timer.update(now);
    CrcLib::Update();
    cmd.refresh();
    if (controller_mode)
    {
        apply_controller_input();
    }
    execute_commands();
    swerve1.update();

    if (read_mode && print_timer.is_time())
    {

        SPRINT("angle:" + String(pwm_enco.getLast().rads, 2));
        SPACER;

        SPRINT("wspeed:" + String(swerve1.get_wheel_rpm(), 2));
        SPACER;
        SPRINT("aspeed:" + String(swerve1._pma._e.getLast().rpm, 2));
        SPACER;
        SPRINT("bspeed:" + String(swerve1._pmb._e.getLast().rpm, 2));
        SPACER;

        SPRINT("[ ");
        SPRINT("s:" + padLeft(String(swerve1._pid.getSetPoint()), 7));
        SPRINT(" ");
        SPRINT("i:" + padLeft(String(swerve1._pid.getInput()), 7));
        SPRINT(" ");
        SPRINT("o:" + padLeft(String(swerve1._pid.getOutput()), 7));
        SPRINT(" ]");
        SPACER;

        SPRINT("[ K ");
        SPRINT(padLeft(String(swerve1._pid.getKp(), 5), 7));
        SPRINT(" ");
        SPRINT(padLeft(String(swerve1._pid.getKi(), 5), 7));
        SPRINT(" ");
        SPRINT(padLeft(String(swerve1._pid.getKd(), 5), 7));
        SPRINT(" ]");
        SPACER;

        Serial.println();
    }
}
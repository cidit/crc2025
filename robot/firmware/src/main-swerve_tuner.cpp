
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
bool read_mode = true;

Timer print_timer(ONE_SECOND / 10), polling_timer(ONE_SECOND / 50);

Motor motorA(CRC_PWM_3);
Encoder encoA(CRC_I2C_SDA, CRC_DIG_4);
GobuildaRotaryEncoder goencoA(encoA, 145.1 * 2.5, polling_timer);
PrecisionMotor pmotorA(motorA, goencoA, 400);

Motor motorB(CRC_PWM_4);
Encoder encoB(CRC_ENCO_A, CRC_DIG_2);
GobuildaRotaryEncoder goencoB(encoB, 145.1 * 2.5, polling_timer);
PrecisionMotor pmotorB(motorB, goencoB, 400);

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco(CRC_DIG_12, MAX_PULSE_LEN, polling_timer);

SwerveModule swerve(pmotorA, pmotorB, pwm_enco);

namespace garbage
{
    // TODO: fix controller, if no input and x-y is 0,
    // angle is automatically 0
    auto controller_mode = false;
    auto last_angle = 0;
    auto last_xy = Vec2D(0, 0);
    // struct controller
    // {
    //     Vec2D right, left;
    // };

    // controller read_controller()
    // {
    //     return (controller){
    //         .right = Vec2D(
    //                      double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X)) / HALF_PWM_OUTPUT,
    //                      double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)) / HALF_PWM_OUTPUT)
    //                      .normalize(),
    //         .left = Vec2D(
    //                     double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X)) / HALF_PWM_OUTPUT,
    //                     double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y)) / HALF_PWM_OUTPUT)
    //                     .normalize()};
    // }

    void update_controller_input()
    {
        last_xy = Vec2D(double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X)) / HALF_PWM_OUTPUT,
                        double(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)) / HALF_PWM_OUTPUT)
                      .normalize();
        if (last_xy.angle() != 0) {
            last_angle = last_xy.angle();
        }
    }

    void apply_controller_input()
    {
        // controller c = !CrcLib::IsCommValid()
        //                    ? (controller){.right = Vec2D(0, 0), .left = Vec2D(0, 0)}
        //                    : read_controller();

        auto sh = (swerve_heading) {
            .direction = last_angle,
            .velocity = last_xy.norm() * 100,
        };

        swerve.set_target(sh);
    }
}

void setup()
{
    Serial.begin(115200);
    CrcLib::Initialize();

    swerve.begin();
    pmotorA._pid_speed.setK(0.60000, 0.00001, 0.12000);
    pmotorB._pid_speed.setK(0.60000, 0.00001, 0.15500);
    swerve.enable(true);

    Serial.println("Setup Done");
}

void execute_commands()
{

    switch (toupper(cmd.getCommand()))
    {
    // TODO: DEPRECATED
    // case 'Z':
    // {
    //     /**
    //      * donne le target en coordonnees cartesiennes
    //      */
    //     Serial.println("WARNING: THE CODE IS NOT OPTIMAL FOR THE SAME REASON THAT THE CONTROLLER DOESNT WORK");
    //     auto target_x_rpm = cmd.getArg(0), target_y_rpm = cmd.getArg(1);
    //     auto target = Vec2D(target_x_rpm, target_y_rpm);
    //     swerve.set_target(target);
    //     Serial.println("Target (@" + String(target.angle()) + ")|(s" + String(target.norm()) + ")");
    //     Serial.println("Target RPM: (vx:" + String(target.x()) + ")|(vy:" + String(target.y()) + ")");
    //     break;
    // }
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
    case 'C':
    {
        garbage::controller_mode = !garbage::controller_mode;
        break;
    }
    }
}

void loop()
{
    auto now = millis();
    print_timer.update(now);
    polling_timer.update(now);
    CrcLib::Update();
    cmd.refresh();

    garbage::update_controller_input();
    if (garbage::controller_mode)
    {
        garbage::apply_controller_input();
    }

    execute_commands();
    swerve.update();

    if (read_mode && print_timer.is_time())
    {

        SPRINT("[ ANGLE ");
        SPRINT("c:" + padRight(String(swerve._e.getLast().rads, 2), 5));
        SPRINT(" ");
        SPRINT("t:" + padRight(String(swerve._target.direction, 2), 5));
        SPRINT(" ]");
        SPACER;

        auto oprev = swerve.get_oprev_result();
        SPRINT("[ OPREV ");
        SPRINT(oprev.reverse ? "Y" : "N");
        SPRINT(" ");
        SPRINT(oprev.travel);
        SPRINT(" ]");
        SPACER;

        SPRINT("[ SPEED ");
        SPRINT("lin: " + String(swerve.get_linear_velocity()));
        SPACER;
        SPRINT("ang: " + String(swerve.get_angular_velocity()));
        SPACER;
        SPRINT("w:" + String(swerve.get_wheel_rpm(), 2));
        SPACER;
        SPRINT("a:" + String(swerve._pma._e.getLast().rpm, 2));
        SPACER;
        SPRINT("b:" + String(swerve._pmb._e.getLast().rpm, 2));
        SPRINT(" ]");
        SPACER;

        SPRINT("[ ");
        SPRINT("s:" + padLeft(String(swerve._pid.getSetPoint()), 7));
        SPRINT(" ");
        SPRINT("i:" + padLeft(String(swerve._pid.getInput()), 7));
        SPRINT(" ");
        SPRINT("o:" + padLeft(String(swerve._pid.getOutput()), 7));
        SPRINT(" ]");
        SPACER;

        SPRINT("[ K ");
        SPRINT(padLeft(String(swerve._pid.getKp(), 5), 7));
        SPRINT(" ");
        SPRINT(padLeft(String(swerve._pid.getKi(), 5), 7));
        SPRINT(" ");
        SPRINT(padLeft(String(swerve._pid.getKd(), 5), 7));
        SPRINT(" ]");
        SPACER;

        Serial.println();
    }
}
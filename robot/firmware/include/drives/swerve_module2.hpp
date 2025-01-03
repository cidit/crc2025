#pragma once
#include "drives/precision_motor2.hpp"
#include "math/vectors.hpp"

using math::cartesian::Vec2D;

class SwerveModule : public Lifecycle
{
public:
    /**
     * the the angle error (in radians, plus or minus) at which the translation speed
     * will be taken into account
     */
    static const auto STEERING_TOLERANCE = .1;

    PrecisionMotor &_pma, &_pmb;
    pin_t _e_p;
    PID_RT _pid; // for angle of swerve
    Vec2D _target;
    double _mtwr;

    SwerveModule(
        PrecisionMotor &pma,
        PrecisionMotor &pmb,
        pin_t abs_enco_pin,
        /**
         * the motor to wheel ratio is how many turns the wheel does
         * for one turn of the ring gear. our ratio is 2/3 because the
         * inner gear has a ratio of 1:2 with the ring gear and the
         * bevel gear on the wheel has a ratio of 3:1 with the inner gear.
         */
        double motor_to_wheel_ratio = 2 / 3)
        : _pma(pma),
          _pmb(pmb),
          _e_p(abs_enco_pin),
          _pid(),
          _target(0, 0),
          _mtwr(motor_to_wheel_ratio)
    {
        // TODO: maybe make a function that resets the pid to a sane state like for the
        // precision motor
        _pid.setInterval(20);
        _pid.setPoint(0);
        _pid.setPropOnError();
        _pid.setReverse(false);
        _pid.setOutputRange(-1, 1);
    }

    void begin() override
    {
        _pma.begin();
        _pmb.begin();
    }

    void update() override
    {
        // TODO: unimplemented
        auto t_angle = _target.angle();
        auto c_angle = get_current_angle();
        auto travel = math::Angle::travel(c_angle, t_angle);
        auto oprev = apply_oprev_optimisation(travel);

        if (!_pid.compute(oprev.travel))
        {
            return;
        }

        auto t_speed = oprev.travel > STEERING_TOLERANCE
                           ? 0
                           : _target.norm() * _mtwr;

        /*
        FORWARD KINEMATICS
        linear velocity: v=(a+b)/2
        angular velocity: Δω=a-b
        REVERSE KINEMATICS
        a=v+(Δω​/2)
        b=v−(Δω/2)
        */
        _set_speeds(
            t_speed + _pid.getOutput() / 2,
            t_speed - _pid.getOutput() / 2);
    }

    /**
     * sets the target angle and speed we want to reach
     * @param target a vec that encodes a direction as an angle and a speed in RPMs
     */
    void set_target(Vec2D target)
    {
        _target = target;
    }

    double get_current_angle()
    {
        // TODO: handle the encoder externally so that the pulse length can be ajusted if need be.
        const auto MAX_PULSE_LEN = 4160.0;
        double pulse = pulseIn(_e_p, HIGH, MAX_PULSE_LEN * 2); // TODO: if there are issues later, check the pulseIn
        return (pulse / MAX_PULSE_LEN) * (2 * M_PI);
    }

    void enable(bool enable)
    {
        _pma.enable(enable);
        _pmb.enable(enable);
        if (enable)
        {
            _pid.start();
        }
        else
        {
            _pid.stop();
        }
    }

    /**
     * immediately sets the speed of the motors.
     */
    void _set_speeds(double rpma, double rpmb)
    {
        _pma.set_target_rpm(rpma);
        _pmb.set_target_rpm(rpmb);
    }
};

/**
 * results of the oposite & reverse optimisation
 */
struct oprev_result
{
    // reversed tells you if the wheel should be reversed or not. (if true, the optimisation was done)
    bool reverse;
    // travel is in angles, gets optimised if reverse is true
    double travel;
};

/**
 * check if can do the oposite & reverse optimisation
 * @param angle zeroed travel angle
 */
oprev_result apply_oprev_optimisation(double angle)
{
    auto reversed = abs(angle) > M_PI_2;
    if (reversed)
    {
        angle += angle > 0
                     ? -M_PI
                     : +M_PI;
    }
    return (oprev_result){
        .reverse = reversed,
        .travel = angle,
    };
}
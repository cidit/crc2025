#pragma once
#include "drives/precision_motor.hpp"
#include "sensors/pwm_rotary_enc.hpp"
#include "math/vectors.hpp"
#include "util/constants.hpp"

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
 * A valid heading for a swerve
 * the equivalent of a vector in polar notation
 *
 * turns out a valid heading cant be expressed in cartesian notation
 * because if the norm is 0 the angle is always going to be 0 (left on
 * the cartesian circle).
 */
struct swerve_heading
{
    double direction; // in rads
    double velocity;  // in rpms
};

/**
 * check if can do the oposite & reverse optimisation
 * @param angle zeroed travel angle
 */
oprev_result apply_oprev_optimisation(double angle)
{
    const auto reversed = abs(angle) > M_PI_2;
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

class SwerveModule : public Lifecycle
{
public:
    /**
     * the the angle error (in radians, plus or minus) at which the translation speed
     * will be taken into account in the swerve.
     */
    static constexpr auto STEERING_TOLERANCE = .3;

    PrecisionMotor &_pma, &_pmb;
    PwmRotaryEncoder &_e;
    PID_RT _pid; // for angular velocity of swerve
    swerve_heading _target;
    double _mtwr;
    bool _enabled;

    SwerveModule(
        PrecisionMotor &pma,
        PrecisionMotor &pmb,
        PwmRotaryEncoder &enco,
        /**
         * the motor to wheel ratio is how many turns the wheel does
         * for one turn of the ring gear. our ratio is 2/3 because the
         * inner gear has a ratio of 1:2 with the ring gear and the
         * bevel gear on the wheel has a ratio of 3:1 with the inner gear.
         */
        double motor_to_wheel_ratio = 2. / 3.)
        : _pma(pma),
          _pmb(pmb),
          _e(enco),
          _pid(),
          _target{.direction = 0, .velocity = 0},
          _mtwr(motor_to_wheel_ratio),
          _enabled(false)
    {
        _pid.setInterval(_e._polling_timer._delay);
        _pid.setPoint(0);
        _pid.setPropOnError();
        _pid.setReverse(true);
        _pid.setOutputRange(-400, 400); // rpms
    }

    SwerveModule(const SwerveModule &) = delete;
    SwerveModule &operator=(const SwerveModule &) = delete;

    void begin() override
    {
        _pma.begin();
        _pmb.begin();
    }

    void update() override
    {
        _e.update();
        const auto oprev = get_oprev_result();

        if (_pid.compute(oprev.travel))
        {
            _update_pmotor_instructions(oprev);
        }

        // put this at the end because we want the pmotors to get the up to date info as soon as possible, instead of next update.
        _pma.update();
        _pmb.update();
    }

    void _update_pmotor_instructions(const oprev_result &oprev) {
        // TODO: come back to the steering tolerance thing
         const auto wheel_velocity = abs(oprev.travel) > STEERING_TOLERANCE
                                 ? 0
                                 : get_linear_velocity();
        // const auto wheel_velocity = cos(oprev.travel) * _target.velocity * _mtwr;

        /*
        FORWARD KINEMATICS
        linear velocity: v=(a+b)/2
        angular velocity: Δω=a-b
        REVERSE KINEMATICS
        a=v+(Δω​/2)
        b=v−(Δω/2)
        */
        const auto angular_velocity = get_angular_velocity(); // rpms

        if (!oprev.reverse)
        {
            _set_speeds(
                -wheel_velocity + (angular_velocity / 2), // in fw, clockwise
                wheel_velocity + (angular_velocity / 2) // counter clockwise
            );
        }
        else
        {
            _set_speeds(
                wheel_velocity + (angular_velocity / 2), // in bckw, counter clockwise
                -wheel_velocity + (angular_velocity / 2)   // clockwise
            );
        }
    }

    /**
     * sets the target angle and speed we want to reach
     * @param target a direction as an angle and a speed in RPMs
     * // TODO: scale the target with some algebric formula to the limit of pmotors?
     */
    void set_target(swerve_heading target)
    {
        _target = target;
    }

    double get_wheel_rpm()
    {
        // TODO: implement
        return 0;
    }

    oprev_result get_oprev_result()
    {
        const auto t_angle = _target.direction;
        const auto c_angle = _e.getLast().rads;
        const auto travel = Angle::travel(c_angle, t_angle);
        return apply_oprev_optimisation(travel);
    }

    /**
     * inspection function for individual motor speed calculation
     */
    double get_angular_velocity()
    {
        return _pid.getOutput();
    }

    double get_max_angular_velocity() {
        return _pid.getOutputMax() / 2; // TODO: div by 2 because the pid output is divided by 2 in the reverse kinematics. investigate if necessary to divide by 2 at all at any point since we are taking angular velocity from the pid anyways (AKA, just reduce proportionnal by 2?)
    }

    /**
     * inspection functions for individual motor speed calculation
     */
    double get_linear_velocity()
    {
        // TODO: should be calculated according to angular distance
        return _target.velocity * _mtwr;
    }

    double get_max_linear_velocity() {
        // TODO: verify if this is actually correct
        return (_pma._max_rpm + _pmb._max_rpm) * _mtwr;
    }

    void enable(const bool enable)
    {
        _enabled = enable;
        _pma.enable(enable);
        _pmb.enable(enable);
        if (enable)
        {
            _pid.start();
        }
        else
        {
            pid_soft_reset(_pid);
        }
    }

    /**
     * immediately sets the speed of the motors.
     */
    void _set_speeds(const double rpma, const double rpmb)
    {
        _pma.set_target_rpm(rpma);
        _pmb.set_target_rpm(rpmb);
    }
};

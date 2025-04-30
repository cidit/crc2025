#pragma once

#include <Arduino.h>
#include <PID_RT.h>
#include <Encoder.h>
#include "sensors/sensor.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "drives/motor.hpp"
#include "math/angles.hpp"
#include "util/lifecycle.hpp"
#include "util/timer.hpp"
#include "util/misc.hpp"
#include "util/display_name.hpp"

/**
 * @class PrecisionMotor
 * @brief A class that provides precise control over a motor using PID controllers.
 * 
 * This class manages a motor with precise angle and speed control using two separate
 * PID controllers. It can operate in two modes: MATCH_ANGLE to maintain a specific
 * angular position, or MATCH_SPEED to maintain a specific rotational speed.
 */
class PrecisionMotor : public Lifecycle, public IDisplayName
{
public:

    /**
     * @enum Mode
     * @brief Defines the operating modes for the PrecisionMotor.
     * 
     * MATCH_ANGLE: Motor will try to maintain a specific angular position.
     * MATCH_SPEED: Motor will try to maintain a specific rotational speed.
     */
    enum class Mode
    {
        MATCH_ANGLE,
        MATCH_SPEED
    };

    /** @brief Name identifier for this motor */
    String _name;

    /** @brief Reference to the underlying motor hardware */
    Motor &_m;
    
    /** @brief Reference to the rotary encoder that provides position feedback */
    GobuildaRotaryEncoder &_e;

    /**
     * @brief PID controller for speed control
     * 
     * INPUT: current rpm
     * SETPOINT: target rpm
     * OUTPUT: power variation. expects between -1000 and 1000 to accommodate KP,KI,KD
     *
     * check constructor for additional base expectations
     */
    PID_RT _pid_speed;
    
    /** @brief Previous encoder values used for calculations */
    int32_t _e_old1, _e_old2;

    /**
     * @brief PID controller for angle control
     * 
     * INPUT: difference between current and target
     * SETPOINT: always 0 because we want to minimize the angle difference
     * OUTPUT: direct power of the motor to reach the desired angle between 0 and 1
     * (can also be interpreted as the power needed to resist change, I.E. this is
     * not a variation of power like it is for the speed)
     *
     * check constructor for additional base expectations
     */
    PID_RT _pid_angle;
    
    /** @brief Target angle the motor should maintain in MATCH_ANGLE mode */
    Angle _target_angle;

    /** @brief Current operating mode (MATCH_ANGLE or MATCH_SPEED) */
    Mode _mode;
    
    /** @brief Flag indicating whether the motor control is enabled */
    bool _enabled;
    
    /** @brief Maximum RPM the motor is allowed to reach */
    double _max_rpm;

    /**
     * @brief Constructor for the PrecisionMotor class
     * 
     * @param name Identifier name for this motor
     * @param m Reference to the motor hardware
     * @param e Reference to the rotary encoder for position feedback
     * @param max_rpm Maximum RPM the motor is allowed to reach
     * 
     * Initializes the PrecisionMotor with default settings and configures the PID controllers
     * with sane default values. The motor starts disabled and in MATCH_ANGLE mode.
     */
    PrecisionMotor(
        String name,
        Motor &m,
        GobuildaRotaryEncoder &e,
        double max_rpm)
        : _name(name),
          _m(m),
          _e(e),
          _pid_speed(),
          _e_old1(0),
          _e_old2(0),
          _pid_angle(),
          _target_angle(Angle::from_rad(0)),
          _mode(Mode::MATCH_ANGLE), // doesnt matter, pids are not started anyways
          _enabled(false),
          _max_rpm(max_rpm)
    {
        // setting sane defaults for our pids
        {
            /* setting up speed pid */
            _pid_speed.setK(0, 0, 0);
            _pid_speed.setInterval(_e._polling_timer._delay);
            _pid_speed.setPoint(0);
            _pid_speed.setPropOnError();
            _pid_speed.setReverse(true);
            // we set the PID output to a big range to make KP,KI,KD bigger
            // numbers. makes the tuning easier for Guillaume.
            _pid_speed.setOutputRange(-1000, 1000); // in rpms
        }
        {
            /* setting up angle pid */
            _pid_angle.setK(0, 0, 0);
            _pid_angle.setInterval(_e._polling_timer._delay);
            _pid_angle.setPoint(0);
            _pid_angle.setPropOnError();
            _pid_angle.setReverse(true);
            _pid_angle.setOutputRange(-1, 1); // in power percentage
        }
    }



    /**
     * @brief Copy constructor (deleted)
     * 
     * Copy construction is disabled to prevent multiple objects controlling the same hardware.
     */
    PrecisionMotor(const PrecisionMotor &) = delete;
    
    /**
     * @brief Assignment operator (deleted)
     * 
     * Assignment is disabled to prevent multiple objects controlling the same hardware.
     */
    PrecisionMotor &operator=(const PrecisionMotor &) = delete;

    /**
     * @brief Returns the display name of this motor
     * 
     * @return String The name of this motor
     * 
     * Implementation of the IDisplayName interface.
     */
    String display_name() {
        return _name;
    }

    /**
     * @brief Initializes the motor and encoder
     * 
     * Calls begin() on both the motor and encoder to initialize them.
     * Implementation of the Lifecycle interface.
     */
    void begin() override
    {
        _m.begin();
        _e.begin();
    }

    /**
     * @brief Updates the motor control based on PID calculations
     * 
     * This function:
     * 1. Updates the encoder readings
     * 2. Computes the PID values based on current mode (angle or speed)
     * 3. Applies the appropriate power to the motor
     * 4. Handles error conditions
     * 
     * Implementation of the Lifecycle interface.
     */
    void update() override
    {
        _e.update();

        const auto speed_compute = _pid_speed.compute(_e.getLast().rpm);
        const auto angle_compute = _pid_angle.compute(
            Angle::travel(_e.getLast().rads, _target_angle));
        
        if (speed_compute && angle_compute)
        {
#ifdef DEBUG
            // TODO: introduce some sort of logger instead?
            Serial.print("!!! invalid state error: both PIDs were computed");
#endif
            enable(false);
            return;
        }

        if (speed_compute)
        {
            const auto new_power = _m.get_power() + _pid_output_to_percentage(_pid_speed);
            _m.set_power_ratio(new_power);
        }
        else if (angle_compute)
        {
            _m.set_power_ratio(_pid_angle.getOutput());
            //Serial.println("out"+ String(_pid_angle.getOutput()));
            if(_name == "Bras Right" || _name == "Bras Left") {
                //Serial.println(_name + " - Last:" + String(_e.getLast().rads)+ "   tar"+ String(_target_angle._radians) + ", power: " + String(_pid_angle.getOutput()));
            }
        }
    }

    /**
     * @brief Sets the target RPM for speed control mode
     * 
     * @param target_rpm The desired rotational speed in RPM
     * 
     * Sets the motor to MATCH_SPEED mode and configures the speed PID controller
     * with the target RPM. The target RPM is constrained to the maximum allowed RPM
     * to prevent integral windup in the PID controller.
     */
    void set_target_rpm(const float target_rpm)
    {
        // constraining to max rpm is important to stop accidentally steep integral creep on the PID.
        const auto rpm = constrain(target_rpm, -_max_rpm, _max_rpm);
        _mode = Mode::MATCH_SPEED;
        _pid_speed.setPoint(rpm);
        _set_active_pid();
    }

    /**
     * @brief Sets the target angle for angle control mode
     * 
     * @param angle The desired angle in radians
     * 
     * Sets the motor to MATCH_ANGLE mode and configures the angle PID controller
     * with the target angle. The angle is validated and converted to an Angle object
     * before being stored.
     */
    void set_target_angle(float angle)
    {
        _mode = Mode::MATCH_ANGLE;
        // validate the angle before saving it
        _target_angle = Angle::from_rad(angle);
        // Serial.println("inAng:"+ String(angle)+ "  tar"+ String(_target_angle._radians));
        _set_active_pid();
    }

    /**
     * @brief Enables or disables the motor control
     * 
     * @param enable True to enable motor control, false to disable
     * 
     * When enabled, activates the appropriate PID controller based on the current mode.
     * When disabled, resets both PID controllers and sets the motor power to zero.
     */
    void enable(bool enable)
    {
        _enabled = enable;
        if (_enabled)
        {
            _set_active_pid();
        }
        else
        {
            pid_soft_reset(_pid_angle);
            pid_soft_reset(_pid_speed);
            _m.set_power_ratio(0);
        }
    }

    /**
     * @brief Converts PID output to a percentage value
     * 
     * @param pid Reference to the PID controller
     * @return double The PID output normalized to a percentage (-1.0 to 1.0)
     * 
     * Converts the raw PID output value to a percentage by dividing by the maximum output.
     * This is primarily used for the speed PID since it has an extended output range.
     * Assumes the output range is centered on zero.
     */
    double _pid_output_to_percentage(PID_RT &pid)
    {
        // FIXME: this is only useful for the speed pid since its the only
        // one that benefits from the extended output range.
        // also, assumes the output range is centered on zero which will always
        // be the case in a precision motor setting.
        return pid.getOutput() / pid.getOutputMax();
    }

    /**
     * @brief Activates the appropriate PID controller based on the current mode
     * 
     * Determines which PID controller should be active based on the current mode,
     * resets the inactive PID controller, and starts the active one if it's not
     * already running.
     * 
     * This ensures that only one PID controller is active at a time, preventing
     * conflicts in motor control.
     */
    void _set_active_pid()
    {
        auto &to_start = _mode == Mode::MATCH_ANGLE
                             ? _pid_angle
                             : _pid_speed;
        auto &to_stop = _mode == Mode::MATCH_ANGLE
                            ? _pid_speed
                            : _pid_angle;

        pid_soft_reset(to_stop);
        if (!to_start.isRunning())
        {
            to_start.start();
        }
    }
};

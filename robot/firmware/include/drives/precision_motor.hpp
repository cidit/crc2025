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

class PrecisionMotor : public Lifecycle
{
public:
    static const uint32_t DEFAULT_POLL_FREQ = 50; // hz

    enum class Mode
    {
        MATCH_ANGLE,
        MATCH_SPEED
    };

    String _name;

    Motor &_m;
    GobuildaRotaryEncoder &_e;

    /**
     * INPUT: current rpm
     * SETPOIT: target rpm
     * OUTPUT: power variation. expects between -1000 and 1000 to accomodate KP,KI,KD
     *
     * check constructor for additionnal base expectations
     */
    PID_RT _pid_speed;
    int32_t _e_old1, _e_old2;

    /**
     * INPUT: difference between current and target
     * SETPOINT: always 0 because we want to minimise the angle difference
     * OUTPUT: direct power of the motor to reach the desired angle between 0 and 1
     * (can also be interpreted as the power needed to resist change, I.E. this is
     * not a variation of power like it is for the speed)
     *
     * check constructor for additionnal base expectations
     */
    PID_RT _pid_angle;
    Angle _target_angle;

    // TODO: remove if integration with externally polled encoder works
    // double _tpt;
    Mode _mode;
    bool _enabled;
    double _max_rpm;

    PrecisionMotor(
        String name,
        Motor &m,
        GobuildaRotaryEncoder &e,
        // TODO: remove if integration with externally polled encoder works
        // double ticks_per_turn,
        double max_rpm)
        : _name(name),
          _m(m),
          _e(e),
          _pid_speed(),
          _e_old1(0),
          _e_old2(0),
          _pid_angle(),
          _target_angle(Angle::from_rad(0)),
          // TODO: remove if integration with externally polled encoder works
          //   _tpt(ticks_per_turn),
          _mode(Mode::MATCH_ANGLE), // doesnt matter, pids are not started anyways
          _enabled(false),
          _max_rpm(max_rpm)
    {
        // setting sane defaults for our pids

        const auto poll_interval = ONE_SECOND / DEFAULT_POLL_FREQ;
        {
            /* setting up speed pid */
            _pid_speed.setK(0, 0, 0);
            _pid_speed.setInterval(poll_interval);
            _pid_speed.setPoint(0);
            _pid_speed.setPropOnError();
            _pid_speed.setReverse(false);
            // we set the PID output to a big range to make KP,KI,KD bigger
            // numbers. makes the tuning easier for Guillaume.
            _pid_speed.setOutputRange(-1000, 1000); // in rpms
        }
        {
            /* setting up angle pid */
            _pid_angle.setK(0, 0, 0);
            _pid_angle.setInterval(poll_interval);
            _pid_angle.setPoint(0);
            _pid_angle.setPropOnError();
            _pid_angle.setReverse(true);
            _pid_angle.setOutputRange(-1, 1); // in power percentage
        }
    }

    PrecisionMotor(const PrecisionMotor &) = delete;
    PrecisionMotor &operator=(const PrecisionMotor &) = delete;

    void begin() override
    {
        _m.begin();
        _e.begin();
    }

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

        // TODO: remove if integration with externally polled encoder works
        // if (speed_compute || angle_compute)
        // {
        //     _update_l2ev();
        // }
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
                Serial.println(_name + " - Last:" + String(_e.getLast().rads)+ "   tar"+ String(_target_angle._radians) + ", power: " + String(_pid_angle.getOutput()));
            }
        }
    }

    // TODO: remove if integration with externally polled encoder works
    // double get_current_rpm()
    // {
    //     // FIXME: will not output correct values if PM is not enabled
    //     // because _update_l2ev() never gets called.
    //     const auto interval = _mode == Mode::MATCH_SPEED
    //                               ? _pid_speed.getInterval()
    //                               : _pid_angle.getInterval();
    //     if (interval == 0) {
    //         return 0;
    //     }
    //     const auto hz = ONE_SECOND / interval;
    //     const auto delay_mins = hz * 60;
    //     return delay_mins * (_delta_ticks() / _tpt);
    // }

    // TODO: remove if integration with externally polled encoder works
    // double get_current_angle()
    // {
    //     const auto e_curr = _e.read();
    //     return Angle::from_ratio(e_curr / _tpt)._radians;
    // }

    void set_target_rpm(const float target_rpm)
    {
        // constraining to max rpm is important to stop accidentally steep integral creep on the PID.
        const auto rpm = constrain(target_rpm, -_max_rpm, _max_rpm);
        _mode = Mode::MATCH_SPEED;
        _pid_speed.setPoint(rpm);
        _set_active_pid();
    }

    void set_target_angle(float angle)
    {
        _mode = Mode::MATCH_ANGLE;
        // validate the angle before saving it
        _target_angle = Angle::from_rad(angle);
        //Serial.println("inAng:"+ String(angle)+ "  tar"+ String(_target_angle._radians));
        _set_active_pid();
    }

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

    // TODO: remove if integration with externally polled encoder works
    // /**
    //  * update last 2 encoder vals
    //  */
    // void _update_l2ev()
    // {
    //     _e_old2 = _e_old1;
    //     _e_old1 = _e.read();
    // }

    // TODO: remove if integration with externally polled encoder works
    // int32_t _delta_ticks()
    // {
    //     return _e_old1 - _e_old2;
    // }

    double _pid_output_to_percentage(PID_RT &pid)
    {
        // FIXME: this is only useful for the speed pid since its the only
        // one that benefits from the extended output range.
        // also, assumes the output range is centered on zero which will always
        // be the case in a precision motor setting.
        return pid.getOutput() / pid.getOutputMax();
    }

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

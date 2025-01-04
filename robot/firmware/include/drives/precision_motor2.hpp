#pragma once

#include <Arduino.h>
#include <PID_RT.h>
#include "sensors/sensor.hpp"
#include "drives/motor.hpp"
#include "math/angles.hpp"
#include "util/lifecycle.hpp"
#include "util/timer.hpp"
#include "util/misc.hpp"
#include <Encoder.h>

class PrecisionMotor : public Lifecycle
{
public:
    static const uint32_t DEFAULT_POLL_FREQ = 50; // hz

    enum class Mode
    {
        MATCH_ANGLE,
        MATCH_SPEED
    };

    Motor &_m;
    Encoder &_e;

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
    double _target_angle;

    double _tpt;
    Mode _mode;
    bool _enabled;
    bool _max_rpm;

    PrecisionMotor(Motor &m, Encoder &e, double ticks_per_turn, double max_rpm)
        : _m(m),
          _e(e),
          _pid_speed(),
          _e_old1(0),
          _e_old2(0),
          _pid_angle(),
          _target_angle(0),
          _tpt(ticks_per_turn),
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
            _pid_speed.setReverse(true);
            // we set the PID output to a big range to make KP,KI,KD bigger
            // numbers. makes the tuning easier for Guillaume.
            _pid_speed.setOutputRange(-1000, 1000);
        }
        {
            /* setting up angle pid */
            _pid_angle.setK(0, 0, 0);
            _pid_angle.setInterval(poll_interval);
            _pid_angle.setPoint(0);
            _pid_angle.setPropOnError();
            _pid_angle.setReverse(false);
            _pid_angle.setOutputRange(-1, 1);
        }
    }

    void begin() override
    {
        _m.begin();
    }

    void update() override
    {
        const auto speed_compute = _pid_speed.compute(get_current_rpm());
        const auto angle_compute = _pid_angle.compute(
            math::Angle::travel(get_current_angle(), _target_angle));

        if (speed_compute && angle_compute)
        {
            #ifdef DEBUG
            // TODO: introduce some sort of logger instead?
            Serial.print("!!! invalid state error: both PIDs were computed");
            #endif DEBUG
            enable(false);
            return;
        }

        if (speed_compute || angle_compute)
        {
            _update_l2ev();
        }
        if (speed_compute)
        {
            const auto new_power = _m.get_power() + _pid_output_to_percentage(_pid_speed);
            _m.set_power_ratio(new_power);
        }
        else if (angle_compute)
        {
            _m.set_power_ratio(_pid_angle.getOutput());
        }
    }

    double get_current_rpm()
    {
        // FIXME: will not output correct values if PM is not enabled
        // because _update_l2ev() never gets called.
        const auto interval = _mode == Mode::MATCH_SPEED
                                  ? _pid_speed.getInterval()
                                  : _pid_angle.getInterval();
        if (interval == 0) {
            return 0;
        }
        const auto hz = ONE_SECOND / interval;
        const auto delay_mins = hz * 60;
        return delay_mins * (_delta_ticks() / _tpt);
    }

    double get_current_angle()
    {
        const auto e_curr = _e.read();
        return math::Angle::from_ratio(e_curr / _tpt)._radians;
    }

    void set_target_rpm(const float rpm)
    {
        // constraining to max rpm is important to stop accidentally steep integral creep on the PID.
        const auto rpm = constrain(rpm, -_max_rpm, _max_rpm);
        _mode = Mode::MATCH_SPEED;
        _pid_speed.setPoint(rpm);
        _set_active_pid();
    }

    void set_target_angle(float angle)
    {
        _mode = Mode::MATCH_ANGLE;
        // validate the angle before saving it
        _target_angle = math::Angle::from_rad(angle)._radians;
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

    /**
     * update last 2 encoder vals
     */
    void _update_l2ev()
    {
        _e_old2 = _e_old1;
        _e_old1 = _e.read();
    }

    int32_t _delta_ticks()
    {
        return _e_old1 - _e_old2;
    }

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

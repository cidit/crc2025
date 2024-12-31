#pragma once

#include <Arduino.h>
#include <PID_RT.h>
#include "sensors/sensor.hpp"
#include "drives/motor.hpp"
#include "math/angles.hpp"
#include "util/looped.hpp"
#include "util/timer.hpp"
#include "util/misc.hpp"
#include <Encoder.h>

namespace drives
{
    class PrecisionMotor2 : public Looped
    {
    public:
        Motor _m;
        Encoder _e;

        /**
         * INPUT: current rpm
         * SETPOIT: target rpm
         * OUTPUT: power variation. expects between -1000 and 1000
         *
         * DOES NOT ASSUME ANYTHING ELSE ABOUT THE PID (should be configured externally)
         */
        PID_RT _pid;

        // Timer _timer; the PID has a builtin timer (.getInterval())

        double _tpt;
        int16_t _target_rpm;
        int32_t _last_2_enco_vals[2];
        boolean _enabled;

        PrecisionMotor2(Motor m, Encoder e, PID_RT pid, double ticks_per_turn)
            : _m(m),
              _e(e),
              _pid(pid),
              _tpt(ticks_per_turn),
              _target_rpm(0),
              _last_2_enco_vals({0, 0}),
              _enabled(false)
        {
            _pid.setOutputRange(-1000, 1000);
        }

        void begin() {
            _m.begin();
            _pid.start();
        }

        void loop() override
        {
            _update_l2ev();
            if (_pid.compute(get_current_rpm()))
            {
                Serial.println("computing...");
                if (!_enabled) {
                    Serial.println("not enabled!!");
                }
                _m.set_power_ratio(_m._last_power + _pid_output_to_percentage());
            }

            if (!_enabled) {
                _m.set_power_ratio(0);
            }
        }

        double get_current_rpm()
        {
            auto hz = ONE_SECOND / _pid.getInterval();
            auto delay_mins = hz * 60;
            return delay_mins * (_delta_ticks() / _tpt);
        }

        void set_target_rpm(float rpm)
        {
            _pid.setPoint(rpm);
        }

        void enable(bool enable)
        {
            _enabled = enable;
        }

        void _update_l2ev()
        {
            auto ltev = _last_2_enco_vals;
            ltev[1] = ltev[0];
            ltev[0] = _e.read();
        }

        int32_t _delta_ticks()
        {
            auto ltev = _last_2_enco_vals;
            return ltev[1] - ltev[0];
        }

        double _pid_output_to_percentage()
        {
            return _pid.getOutput() / 1000;
        }
    };

} // namespace drives

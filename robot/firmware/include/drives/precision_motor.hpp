#pragma once

#include <PID_v1.h>
#include "sensors/sensor.hpp"
#include "drives/motor.hpp"
#include "math/angles.hpp"
#include "util/looped.hpp"
#include "util/timer.hpp"
#include "util/misc.hpp"

// Define the aggressive and conservative Tuning Parameters
double aggKp = 4, aggKi = 0.2, aggKd = 1;
double consKp = 1, consKi = 0.05, consKd = 0.25;

namespace drives
{

    // TAKEN FROM EXAMPLE `PID_Basic.ino`
    const double Kp = 2, Ki = 5, Kd = 1;

    void pid_set_conservative(PID &pid)
    {
        pid.SetTunings(consKp, consKi, consKd);
    }
    void pid_set_agressive(PID &pid)
    {
        pid.SetTunings(aggKp, aggKi, aggKd);
    }

    class PrecisionMotor : public Looped
    {

    public:
        /**
         * depending on which mode the precision motor is on, it will use the encoder differently.
         */
        enum class Mode : int
        {
            /**
             * when matching rpm, the motor will output as much power as needed to maintain a certain speed.
             */
            MATCH_RPM = 0,
            /**
             * when matching angle, the motor will apply oposite forces to maintain the specific angle of the wheel.
             */
            MATCH_ANGLE = 1,
        };

        Mode _mode;
        Motor _motor;
        sensors::Sensor<math::Angle> &_encoder;
        PID _pid;
        math::Angle _last_polled_position;

        /**
         * These are the parameters for the PID.
         */
        double _setpoint, _input, _output;
        double _ticks_per_rotation, current_rpm;

        math::Angle _target_angle;
        double _target_rps;

        /**
         * handles how often the encoder will be polled.
         *
         * poll rate must be fast enough so that the wheel doesnt have the time make more than half a turn.
         */
        Timer _polling_timer;

        PrecisionMotor(Motor m,
                       sensors::Sensor<math::Angle> &e,
                       PID pid,
                       uint16_t poll_rate)
            : _mode(Mode::MATCH_ANGLE),
              _motor(m),
              _encoder(e),
              _pid(pid),
              _last_polled_position(e.getLast()),
              _polling_timer(ONE_SECOND / poll_rate)
        {
            _pid.SetMode(AUTOMATIC);
            _pid.SetOutputLimits(-1, 1);
            _pid.SetSampleTime(_polling_timer._delay);
            set_target_angle(math::Angle::zero()); // initialize for angle control
        }

        PrecisionMotor(Motor m, sensors::Sensor<math::Angle> &e)
            : PrecisionMotor(
                  m, e,
                  PID(&_input, &_output, &_setpoint, Kp, Ki, Kd, DIRECT),
                  100) {}

        virtual void loop() override
        {
            _motor.loop();
            auto now = millis();

            if (!_polling_timer.is_time(now))
            {
                return;
            }

            _encoder.poll();
            if (!_pid.Compute())
            {
                Serial.println("!? - pid didnt compute, but should have.");
            }

            if (_mode == Mode::MATCH_ANGLE)
            {
                _input = math::Angle::travel(_encoder.getLast(), _target_angle);
                motor.set_speed(_output);
            }

            if (_mode == Mode::MATCH_RPM)
            {
                auto distance_travelled = math::Angle::travel(_last_polled_position, _encoder.getLast());
                auto current_rpm = distance_travelled * ONE_SECOND/_polling_timer._delay;
                // TODO: now do the prop on measurements PID thing

                
                // math::Angle new_a, old_a = _encoder.getLast();
                // _encoder.sample(new_a);
                // auto travel = math::Angle::travel(old_a, new_a);
                // current_rpm = travel * (1000 / _polling_timer._delay) * (60 * ONE_SECOND);
            }

            // _motor.set_speed(_output/OUTPUT_LIM);
        }

        void set_target_angle(math::Angle angle)
        {
            _mode = Mode::MATCH_ANGLE;
            _target_angle = angle;
            _setpoint = 0;
        }

        void set_target_speed(double rpm)
        {
            _mode = Mode::MATCH_RPM;
            _target_rps = _setpoint = rpm;
        }
    };

}
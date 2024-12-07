#pragma once

#include <PID_v1.h>
#include "drives/motor.hpp"
#include "sensors/rotary_encoder.hpp"
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

        Motor _motor;
        sensors::Sensor<math::Angle> &_encoder;
        PID _pid;
        Mode _mode;

        /**
         * These are the parameters for the PID.
         * `_setpoint` should always be 0 and represents the target direction of the wheel.
         * `_input` represents the angular disatance between the wheel position and where we want it to go when in
         *          MATCH_ANGLE mode and the expected RPM in MATCH_SPEED mode
         * `_output` represents the strength at which the motor must move to reach the position.
         */
        double _setpoint, _input, _output;
        double _ticks_per_rotation, current_rpm;

        math::Angle _target_angle;
        double _target_rpm;

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
              _polling_timer(ONE_SECOND / poll_rate)
        {
            _pid.SetMode(AUTOMATIC); // turns the PID on.

            _pid.SetOutputLimits(-HALF_PWM_OUTPUT, HALF_PWM_OUTPUT); // limits for the vex, will be
            _pid.SetSampleTime(_polling_timer._delay);
            pid_set_agressive(_pid);
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

            if (_mode == Mode::MATCH_ANGLE)
            {
                math::Angle current_angle;
                _encoder.sample(current_angle);

                const auto to_travel = math::Angle::travel(current_angle, _target_angle);
                _input = to_travel - PI;
                _pid.Compute();
            }

            if (_mode == Mode::MATCH_RPM)
            {
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
            // TODO: unimplemented
        }

        void set_target_speed(double rpm)
        {
            _mode = Mode::MATCH_RPM;
            _target_rpm = rpm;
            _setpoint = rpm;
            // TODO: unimplemented
        }
    };

}
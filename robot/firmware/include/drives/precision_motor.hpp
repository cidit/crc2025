#pragma once

#include <PID_v1.h>
#include "drives/motor.hpp"
#include "sensors/rotary_encoder.hpp"
#include "math/angles.hpp"
#include "util/looped.hpp"
#include "util/timer.hpp"

// Define the aggressive and conservative Tuning Parameters
double aggKp = 4, aggKi = 0.2, aggKd = 1;
double consKp = 1, consKi = 0.05, consKd = 0.25;

namespace drives
{

    // TAKEN FROM EXAMPLE `PID_Basic.ino`
    const double Kp = 2, Ki = 5, Kd = 1;

    /**
     * depending on which mode the precision motor is on, it will use the encoder differently.
     */
    enum class PMMode : int
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

    class PrecisionMotor : public Looped
    {
        PMMode _mode;

    public:
        Motor _motor;
        sensors::RotaryEncoder _encoder;
        PID _pid;

        /**
         * These are the parameters for the PID.
         * `_setpoint` should always be 0 and represents the target direction of the wheel.
         * `_input` represents the angular disatance between the current
         * `_output` represents the strength at which the motor must move to reach the position.
         */
        double _setpoint, _input, _output;
        double _ticks_per_rotation, current_rpm;

        /**
         * handles how often the encoder will be polled.
         *
         * poll rate must be fast enough so that the wheel doesnt have the time make more than half a turn.
         */
        Timer _polling_timer;

        void pid_set_agressive(PID &pid)
        {
            pid.SetTunings(aggKp, aggKi, aggKd);
        }

        PrecisionMotor(Motor m,
                       sensors::RotaryEncoder e,
                       PID pid,
                       uint16_t poll_rate)
            : _mode(PMMode::MATCH_ANGLE),
              _motor(m),
              _encoder(e),
              _pid(pid),
              _polling_timer(ONE_SECOND / poll_rate)
        {
            _pid.SetMode(AUTOMATIC); // turns the PID on.
            pid_set_agressive(_pid);
        }

        PrecisionMotor(Motor m, sensors::RotaryEncoder e)
            : PrecisionMotor(
                  m, e,
                  PID(&_input, &_output, &_setpoint, Kp, Ki, Kd, DIRECT),
                  100) {}

        virtual void loop() override
        {
            const int INPUT_LIM = 1024; //, OUPUT_LIM = 255;
            auto now = millis();

            // _encoder.poll();
            // TODO: input should not be position, but distance from target angle
            _input = _encoder.getLast().ratio() * INPUT_LIM;
            _pid.Compute();

            if (_polling_timer.time(now))
            {
                math::Angle new_a, old_a = _encoder.getLast();
                _encoder.sample(new_a);
                auto travel = math::Angle::travel(old_a, new_a);
                current_rpm = travel * (1000 / _polling_timer._delay) * (60 * ONE_SECOND);
            }

            if (_mode == PMMode::MATCH_ANGLE)
            {
            }
            if (_mode == PMMode::MATCH_RPM)
            {
            }

            // _motor.set_speed(_output/OUTPUT_LIM);
        }

        void set_target_angle(math::Angle angle)
        {
            // const int INPUT_LIM = 1024, OUPUT_LIM = 255;
            _mode = PMMode::MATCH_ANGLE;
            _setpoint = 0;
            // TODO: unimplemented
        }

        void set_target_speed(double rpm)
        {
            _mode = PMMode::MATCH_RPM;
            _setpoint = rpm;
            // TODO: unimplemented
        }

        void set_zero(int ticks)
        {
            // TODO: unimplemented
        }

        void set_enc_0()
        {
            _encoder._internal_encoder.write(0);
        }
    };

    void pid_set_conservative(PID &pid)
    {
        pid.SetTunings(consKp, consKi, consKd);
    }
}
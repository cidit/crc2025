#pragma once

#include "drives/motor.hpp"
#include "sensors/rotary_encoder.hpp"
#include "math/angles.hpp"
#include <PID_v1.h>

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
    enum PMMode : int
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
    public:
        PMMode _mode;
        Motor _motor;
        sensors::RotaryEncoder _encoder;
        PID _pid;
        double _setpoint, _input, _output, _ticks_per_rotation;

        PrecisionMotor(Motor m, sensors::RotaryEncoder e, PID pid)
            : _mode(PMMode::MATCH_ANGLE),
              _motor(m),
              _encoder(e),
              _pid(pid)
        {
            _pid.SetMode(AUTOMATIC); // turns the PID on.
            pid_set_agressive(_pid);
        }

        PrecisionMotor(Motor m, sensors::RotaryEncoder e)
            : PrecisionMotor(
                  m, e,
                  PID(&_input, &_output, &_setpoint, Kp, Ki, Kd, DIRECT)) {}

        virtual void loop() override
        {
            const int INPUT_LIM = 1024, OUPUT_LIM = 255;
            
            _encoder.poll();
            // TODO: input should not be position, but distance from target angle
            _input = _encoder.getLast().ratio() * INPUT_LIM;
            _pid.Compute();
            // _motor.set_speed(_output/OUTPUT_LIM);
        }

        void set_target_angle(math::Angle angle)
        {
            const int INPUT_LIM = 1024, OUPUT_LIM = 255;
            _mode = PMMode::MATCH_ANGLE;
            _setpoint = angle.ratio()*INPUT_LIM;
            // TODO: unimplemented
        }

        void set_target_speed(int rpm)
        {
            _mode = PMMode::MATCH_RPM;
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

    void pid_set_agressive(PID &pid)
    {
        pid.SetTunings(aggKp, aggKi, aggKd);
    }
    void pid_set_conservative(PID &pid)
    {
        pid.SetTunings(consKp, consKi, consKd);
    }
}
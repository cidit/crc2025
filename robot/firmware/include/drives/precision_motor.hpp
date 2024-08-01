#pragma once

#include "drives/motor.hpp"
#include "sensors/encoder.hpp"
#include "math/angles.hpp"
#include <PID_v1.h>

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

class PrecisionMotor
{
    PMMode _mode;
    Motor _motor;
    RotaryEncoder _encoder;
    PID _pid;
    double _setpoint, _input, _output, _ticks_per_rotation;

public:
    PrecisionMotor(Motor m, RotaryEncoder e)
        : _mode(PMMode::MATCH_ANGLE),
          _motor(m),
          _encoder(e),
          _pid(PID(&_input, &_output, &_setpoint, Kp, Ki, Kd, DIRECT)) {
          };

    PrecisionMotor(Motor m, RotaryEncoder e, PID pid)
        : _mode(PMMode::MATCH_ANGLE),
          _motor(m),
          _encoder(e),
          _pid(pid){
          };

    void set_zero(int ticks)
    {
        // TODO: unimplemented
    }
    void set_target_angle(Angle angle)
    {
        _mode = PMMode::MATCH_ANGLE;
        // TODO: unimplemented
    }
    void set_target_speed(int rpm)
    {
        _mode = PMMode::MATCH_RPM;
        // TODO: unimplemented
    }
    void set_enc_0()
    {
        _encoder._internal_encoder.write(0);
    }
};
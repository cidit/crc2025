#pragma once

#include "drives/motor.hpp"
#include "sensors/encoder.hpp"
#include <PID_v1.h>

class PrecisionMotor
{
    Motor _motor;
    Encoder _encoder;
    PID _pid;
public:
    PrecisionMotor(Motor, Encoder, PID, int ticks_per_full_rotation);
    void set_target_angle(int angle);
    void set_target_speed(int speed);
};
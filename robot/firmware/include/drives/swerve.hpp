#pragma once

#include "drives/precision_motor.hpp"
#include "sensors/encoder.hpp"

typedef struct IntVector
{
    int x, y;
} IntVector;

class Swerve
{
    PrecisionMotor direction_motor, speed_motor;

public:
    Swerve();
    void set_target_movement_vector(IntVector);
};
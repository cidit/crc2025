#pragma once

#include "drives/precision_motor.hpp"
#include "sensors/encoder.hpp"
#include "math/vectors.hpp"
#include "math/angles.hpp"

class Swerve
{
    PrecisionMotor _direction_motor, _speed_motor;

public:
    Swerve(
        PrecisionMotor direction_motor, 
        PrecisionMotor speed_motor
    ): _direction_motor(direction_motor), _speed_motor(speed_motor) 
    {};
    void aim_towards(Angle angle) {
        _direction_motor.set_target_angle(angle);
    }
    // void set_target_movement_vector(math::cartesian::Vec3D);
};
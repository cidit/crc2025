#pragma once

#include "drives/precision_motor.hpp"
#include "sensors/rotary_encoder.hpp"
#include "math/vectors.hpp"
#include "math/angles.hpp"
#include <cmath>
#include "util.hpp"

namespace drives
{

    class Swerve: public Looped
    {
        PrecisionMotor _direction_motor, _speed_motor;
        math::cartesian::Vec3D _target_movement;

    public:
        Swerve(PrecisionMotor direction_motor,
               PrecisionMotor speed_motor)
            : _direction_motor(direction_motor),
              _speed_motor(speed_motor),
              _target_movement(math::cartesian::Vec3D::zero()) {};
        
        virtual void loop() override {
            _direction_motor.loop();
            _speed_motor.loop();
        }

        void aim_towards(math::Angle angle)
        {
            // _target_movement =
        }
        
        void set_target_movement(const math::cartesian::Vec3D mvmt)
        {
            _target_movement = mvmt;
        }
    };
}
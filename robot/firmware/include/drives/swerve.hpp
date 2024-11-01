#pragma once

#include "drives/precision_motor.hpp"
#include "sensors/rotary_encoder.hpp"
#include "math/vectors.hpp"
#include "math/angles.hpp"
#include <cmath>
#include "util/misc.hpp"

namespace drives
{

    class Swerve: public Looped
    {
        PrecisionMotor a_motor, b_motor;
        math::cartesian::Vec3D _target_movement;

    public:
        Swerve(PrecisionMotor a_motor,
               PrecisionMotor b_motor)
            : a_motor(a_motor),
              b_motor(b_motor),
              _target_movement(math::cartesian::Vec3D::zero()) {};
        
        virtual void loop() override {
            a_motor.loop();
            b_motor.loop();
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
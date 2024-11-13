#pragma once

#include <cmath>
#include <Arduino.h>
#include "util/misc.hpp"
#include "math/angles.hpp"
#include "sensors/sensor.hpp"
#include "sensors/linear_encoder.hpp"

namespace sensors
{    
    math::Angle ticks_to_angle(float ticks, float ticks_per_rotation)
    {
        auto wrapped = fmod(ticks, ticks_per_rotation);
        auto ratio = wrapped / ticks_per_rotation;
        return math::Angle::from_ratio(ratio);
    }

    class RotaryEncoder : public Sensor<math::Angle>
    {
    public:
        LinearEncoder _internal_encoder;
        const double _ticks_per_rotation;

        RotaryEncoder(pin_t clock, pin_t clock_offset, double ticks_per_rotation)
            : Sensor(math::Angle::zero()),
              _internal_encoder(LinearEncoder(clock, clock_offset)),
              _ticks_per_rotation(ticks_per_rotation) {}

        RotaryEncoder(LinearEncoder internal_encoder, double ticks_per_rotation)
            : Sensor(math::Angle::zero()),
              _internal_encoder(internal_encoder),
              _ticks_per_rotation(ticks_per_rotation) {}

        void begin() override
        {
            // no-op
        }

        bool sample(math::Angle &out) override
        {
            int internal_counter;
            _internal_encoder.sample(internal_counter);
            out = ticks_to_angle(internal_counter, _ticks_per_rotation);
            return true;
        }
    };

} // namespace sensors
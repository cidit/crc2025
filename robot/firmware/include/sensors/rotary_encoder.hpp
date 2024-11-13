#pragma once

#include <cmath>
#include <Arduino.h>
#include "util/misc.hpp"
#include <Encoder.h>
#include "math/angles.hpp"
#include "sensor.hpp"

namespace sensors
{

    // TODO: probably a bad idea actually.
    // struct RotaryEncoderData
    // {
    //     Direction direction;
    //     uint32_t speed; // rpm
    //     math::Angle angle;    // radians
    // };

    math::Angle ticks_to_angle(int ticks, int ticks_per_rotation)
    {
        auto wrapped = fmod(ticks, ticks_per_rotation);
        auto ratio = wrapped / ticks_per_rotation;
        return math::Angle::from_ratio(ratio);
    }

    class RotaryEncoder : public Sensor<math::Angle>
    {
    public:
        Encoder _internal_encoder;
        const double _ticks_per_rotation;

        RotaryEncoder(pin_t clock, pin_t clock_offset, double ticks_per_rotation)
            : Sensor(math::Angle::zero()),
              _internal_encoder(Encoder(clock, clock_offset)),
              _ticks_per_rotation(ticks_per_rotation) {}

        RotaryEncoder(Encoder internal_encoder, double ticks_per_rotation)
            : Sensor(math::Angle::zero()),
              _internal_encoder(internal_encoder),
              _ticks_per_rotation(ticks_per_rotation) {}

        void begin() override
        {
            // no-op
        }

        bool sample(math::Angle &out) override
        {
            auto internal_counter = _internal_encoder.read();
            out = ticks_to_angle(internal_counter, _ticks_per_rotation);
            return true;
        }
    };

} // namespace sensors
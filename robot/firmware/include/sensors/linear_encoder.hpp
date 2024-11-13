#pragma once

#include <sensors/sensor.hpp>
#include <util/misc.hpp>
#include <Encoder.h>
using UnderlyingEncoder = Encoder;


namespace sensors
{
    class LinearEncoder : public sensors::Sensor<int>
    {
        UnderlyingEncoder _underlying_encoder;

    public:
        LinearEncoder(pin_t clock, pin_t clock_offset)
            : Sensor(0), _underlying_encoder(Encoder(clock, clock_offset))
        {}

        void begin() override
        {
            // no-op
        }

        bool sample(int &out) override
        {
            out = _underlying_encoder.read();
            return true;
        }
    };
} // namespace sensors

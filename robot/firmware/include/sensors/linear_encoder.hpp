#pragma once

#include <sensors/sensor.hpp>

#ifdef PLTFRM_ESP32
#include <ESP32Encoder.h>
using UnderlyingEncoder = ESP32Encoder;
#else
#include <Encoder.h>
using UnderlyingEncoder = Encoder;
#endif
#include <util/misc.hpp>

namespace sensors
{
    class LinearEncoder : public sensors::Sensor<int>
    {
        UnderlyingEncoder _underlying_encoder;

    public:
        LinearEncoder(pin_t clock, pin_t clock_offset)
            : Sensor(0)
        {
#ifdef PLTFRM_ESP32
            this->_underlying_encoder = ESP32Encoder(true);
            this->_underlying_encoder.attachFullQuad(clock, clock_offset);
            this->_underlying_encoder.setCount(0);
#else
            this->_underlying_encoder = Encoder(clock, clock_offset);
#endif
        }

        void begin() override
        {
            // no-op
        }

        bool sample(int &out) override
        {
#ifdef PLTFRM_ESP32
            out = _underlying_encoder.getCount();
#else
            out = _underlying_encoder.read();
#endif
            return true;
        }
    };
} // namespace sensors

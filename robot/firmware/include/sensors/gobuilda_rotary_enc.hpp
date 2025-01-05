#pragma once

#include <cmath>
#include <Arduino.h>
#include <Encoder.h>
#include "util/misc.hpp"
#include "math/angles.hpp"
#include "sensors/sensor.hpp"

class GobuildaRotaryEnco : public Sensor<Angle>
{
public:
    //---------------------- CONSTRUCTORS ---------------------------
    GobuildaRotaryEnco(Encoder internal_encoder, double ticks_per_rotation)
        : Sensor(Angle::zero()),
          _internal_encoder(internal_encoder),
          _ticks_per_rotation(ticks_per_rotation) {}

    GobuildaRotaryEnco(pin_t clock, pin_t clock_offset, double ticks_per_rotation)
        : GobuildaRotaryEnco(Encoder(clock, clock_offset), ticks_per_rotation) {}

    //-------------------------- FUNCTIONS -----------------------------
    void begin() override
    {
        // no-op
    }

    bool sample(Angle &out) override
    {
        int internal_counter = _internal_encoder.read();
        out = ticks_to_angle(internal_counter);
        return true;
    }

    /**
     * Convert the number of tick of the encoder to an angle
     */
    Angle ticks_to_angle(float ticks)
    {
        auto wrapped = fmod(ticks, _ticks_per_rotation);
        auto ratio = wrapped / _ticks_per_rotation;
        // Serial.println("w: r:" +  String(wrapped) + " " + String(ratio));
        return Angle::from_ratio(ratio);
    }

private:
    //-------------------------- VARIABLES -----------------------------
    Encoder &_internal_encoder;
    const double _ticks_per_rotation;
};

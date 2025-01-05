#pragma once

#include <cmath>
#include <Arduino.h>
#include <Encoder.h>
#include "util/misc.hpp"
#include "math/angles.hpp"
#include "sensors/sensor.hpp"

struct GobuildaRotaryEncoderData
{
    double rads;
    double rpm;
};

class GobuildaRotaryEncoder : public Sensor<GobuildaRotaryEncoderData>
{
public:
    Encoder &_ie;
    const double _tpt;

    GobuildaRotaryEncoder(
        Encoder &internal_encoder,
        double ticks_per_TURN,
        const Timer &polling_timer)
        : Sensor({
                     .rads = 0,
                     .rpm = 0,
                 },
                 polling_timer),
          _ie(internal_encoder), _tpt(ticks_per_TURN)
    {
    }

    void begin() override
    {
        _ie.write(0);
    }

    void reset()
    {
        _ie.write(0);
        _last = {0, 0};
    }

    bool sample(GobuildaRotaryEncoderData &out) override
    {
        int32_t internal_counter = _ie.read();
        const auto angle = _ticks_to_angle(internal_counter);
        const auto freq = ONE_SECOND / _polling_timer._delay;
        const auto freq_per_minute = freq * 60;
        const auto num_rotations = (angle._radians - getLast().rads) / M_2_PI;
        out = {
            .rads = angle._radians,
            .rpm = num_rotations / freq_per_minute};
        return true;
    }

    /**
     * Convert the number of tick of the encoder to an angle
     */
    Angle _ticks_to_angle(float ticks)
    {
        auto wrapped = fmod(ticks, _tpt);
        auto ratio = wrapped / _tpt;
        return Angle::from_ratio(ratio);
    }
};

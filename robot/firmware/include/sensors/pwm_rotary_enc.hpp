#pragma once

#include <Arduino.h>
#include "util/misc.hpp"
#include "util/timer.hpp"
#include "sensors/sensor.hpp"
#include "math/angles.hpp"

struct PwmRotaryEncoderData {
    double rads;
};

class PwmRotaryEncoder : public Sensor<PwmRotaryEncoderData>
{
    pin_t _p;
    double _mpl, _offset;

public:
    PwmRotaryEncoder(pin_t pin, double max_pulse_length, double offset, const Timer &poll_timer)
        : Sensor({0}, poll_timer),
          _p(pin), _mpl(max_pulse_length), _offset(offset)
    {
    }

    void begin() override
    {
        // no-op
    }

    bool sample(PwmRotaryEncoderData &out) override {
        const double pulse = pulseIn(_p, HIGH, _mpl * 2); // TODO: if there are issues later, check the pulseIn
        const double angle = (pulse / _mpl) * (2 * M_PI);
        const double offsetted = Angle::from_rad(angle + _offset)._radians;
        out = {
            .rads = offsetted
        };
        return true;
    }

};
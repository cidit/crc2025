#pragma once

#include <Arduino.h>
#include "util/misc.hpp"
#include "util/timer.hpp"
#include "sensors/sensor.hpp"

struct PwmRotaryEncoderData: public Printable {
    double rads;

    virtual size_t printTo(Print &p) const override
    {
        return p.print("[@"+String(rads)+"]");
    }
};

class PwmRotaryEncoder : public Sensor<PwmRotaryEncoderData>
{
    pin_t _p;
    double _mpl;

public:
    PwmRotaryEncoder(pin_t pin, double max_pulse_length, const Timer &poll_timer)
        : Sensor({0}, poll_timer),
          _p(pin), _mpl(max_pulse_length)
    {
    }

    void begin() override
    {
        // no-op
    }

    bool sample(PwmRotaryEncoderData &out) override {
        const double pulse = pulseIn(_p, HIGH, _mpl * 2); // TODO: if there are issues later, check the pulseIn
        const double angle = (pulse / _mpl) * (2 * M_PI);
        out = {
            .rads = angle
        };
        return true;
    }

};
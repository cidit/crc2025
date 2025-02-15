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
public:
    pin_t _p;
    double _mpl, _offset;
    bool _inverted;

    /**
     * absolute pwm-based encoder. deals in rads.
     * 
     * @param pin the pin to to read from
     * @param max_pulse_length the expected maximum pulse length in microseconds
     * @param the offset from zero in rads
     * @param poll_timer an externally managed timer that dictates when the sensor should poll
     */
    PwmRotaryEncoder(pin_t pin, double max_pulse_length, double offset, const Timer &poll_timer, bool inverted = false)
        : Sensor({0}, poll_timer),
          _p(pin), _mpl(max_pulse_length), _offset(offset), _inverted(inverted)
    {
    }

    PwmRotaryEncoder(const PwmRotaryEncoder &) = delete;
    PwmRotaryEncoder &operator=(const PwmRotaryEncoder &) = delete;

    void begin() override
    {
        // no-op
    }

    bool sample(PwmRotaryEncoderData &out) override {
        const double pulse = pulseIn(_p, HIGH); // TODO: if there are issues later, check the pulseIn
        const double mb_inverted = _inverted? _mpl-pulse : pulse;
        const double angle = (mb_inverted / _mpl) * (2 * M_PI);
        const double offsetted = Angle::from_rad(angle + _offset)._radians;
        out = {
            .rads = offsetted
        };
        return true;
    }

    void set_inverted(bool inverted) {
        _inverted = inverted;
    }

    void set_offset(double offset_rads) {
        _offset = offset_rads;
    }

};
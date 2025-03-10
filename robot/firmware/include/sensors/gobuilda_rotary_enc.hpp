#pragma once

#include <Arduino.h>
#include <Encoder.h>
#include "util/misc.hpp"
#include "math/angles.hpp"
#include "sensors/sensor.hpp"
#include "sensors/gobuilda_rotary_enc_data.hpp"

class GobuildaRotaryEncoder : public Sensor<GobuildaRotaryEncoderData>
{
public:
    Sensor<int32_t> &_ie;
    int32_t _ie_current, _ie_old;
    const double _tpt;
    bool _is_inverted;

    GobuildaRotaryEncoder(
        Sensor<int32_t> &internal_encoder,
        double ticks_per_turn,
        const Timer &polling_timer,
        const bool reversed = false)
        : Sensor({
                     .rads = 0,
                     .rpm = 0,
                 },
                 polling_timer),
          _ie(internal_encoder), _ie_current(0), _ie_old(0), _tpt(ticks_per_turn), _is_inverted(reversed)
    {
    }

    GobuildaRotaryEncoder(const GobuildaRotaryEncoder &) = delete;
    GobuildaRotaryEncoder &operator=(const GobuildaRotaryEncoder &) = delete;

    void begin() override
    {
        // TODO: keep track of an offset in this class instead and update it to the current value here.
        // _ie.write(0);
        _ie.begin();
    }

    void reset()
    {
        // TODO: keep track of an offset instead like explained in begin()
        // _ie.write(0);
        _last = {0, 0};
    }

    void set_inverted(bool inverted) {
        _is_inverted = inverted;
    }

    bool sample(GobuildaRotaryEncoderData &out) override
    {
        const auto angle = _ticks_to_angle(_internal_read());
        const auto freq = _polling_timer._delay == 0
                              ? 0
                              : ONE_SECOND / _polling_timer._delay;
        const auto freq_per_minute = freq * 60; // hz * 60seconds
        const auto num_rotations = double(_ie_current - _ie_old) / _tpt;
        out = {
            .rads = angle._radians,
            .rpm = num_rotations * freq_per_minute};
        return true;
    }

    void poll() override
    {
        // speed will only calculate correctly if we poll, which makes sense.
        _ie_old = _ie_current;
        _ie_current = _internal_read();

        Sensor::poll();
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

    int32_t _internal_read() {
        auto read = _ie.getLast();
        return _is_inverted? -read: read;
    }

    void update() override {
        _ie.update();
        Sensor<GobuildaRotaryEncoderData>::update();
    }
};

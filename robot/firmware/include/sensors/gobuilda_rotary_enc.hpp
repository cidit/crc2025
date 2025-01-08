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
    int32_t _ie_current, _ie_old;
    const double _tpt;

    GobuildaRotaryEncoder(
        Encoder &internal_encoder,
        double ticks_per_turn,
        const Timer &polling_timer)
        : Sensor({
                     .rads = 0,
                     .rpm = 0,
                 },
                 polling_timer),
          _ie(internal_encoder), 
          _ie_current(0), 
          _ie_old(0), 
          _tpt(ticks_per_turn)
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
        const auto angle = _ticks_to_angle(_ie.read());
        const auto freq = _polling_timer._delay == 0 
            ? 0 
            : ONE_SECOND / _polling_timer._delay;
        const auto freq_per_minute = freq * 60; // hz * 60seconds
        const auto num_rotations = double(_ie_current-_ie_old) / _tpt;
        Serial.print("|");
        Serial.print("dt:" + String(_ie_current-_ie_old));
        Serial.print("nr:" + String(num_rotations, 5) + "|");
        Serial.print("fm:" + String(freq_per_minute, 5)+ "|");
        out = {
            .rads = angle._radians,
            .rpm = num_rotations * freq_per_minute};
        return true;
    }

    void poll() override {
        // speed will only calculate correctly if we poll, which makes sense.
        _ie_old = _ie_current;
        _ie_current = _ie.read();

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
};

#pragma once

#include <Encoder.h>
#include <sensors/sensor.hpp>
#include <util/timer.hpp>

class EncoderWrapper : public Sensor<int32_t>
{
    public:
    Encoder &_ie;

    EncoderWrapper(
        Encoder &internal,
        Timer &polling_timer)
        : Sensor(0, polling_timer), _ie(internal)
    {
    }

    EncoderWrapper(const EncoderWrapper &) = delete;
    EncoderWrapper &operator=(const EncoderWrapper &) = delete;

    void begin() override { /* no-op*/ };
    bool sample(int32_t &out)
    {
        out = _ie.read();
        return true;
    }
};
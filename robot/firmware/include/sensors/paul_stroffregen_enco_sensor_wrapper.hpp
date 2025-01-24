#pragma once

#include <Encoder.h>
#include <sensors/sensor.hpp>
#include <util/timer.hpp>

class EncoderWrapper : public Sensor<int32_t>
{

    Encoder &_ie;

    EncoderWrapper(
        Encoder &internal,
        Timer &polling_timer)
        : Sensor(0, polling_timer), _ie(internal)
    {
    }

    void begin() override { /* no-op*/ };
    bool sample(int32_t &out)
    {
        out = _ie.read();
    }
};
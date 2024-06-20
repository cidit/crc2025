#pragma once
#include "sensor.hpp"
#include <Arduino.h>
#include "util_defs.hpp"
#include <Encoder.h>

enum Direction
{
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

struct EncoderData
{
    Direction direction;
    uint32_t rpm;
};

class EncoderSensor : Sensor<EncoderData>
{
    Encoder _internal_encoder;

public:
    EncoderSensor(pin_t clock, pin_t clock_offset);
    void begin() override;
    bool sample(EncoderData &out) override;
};
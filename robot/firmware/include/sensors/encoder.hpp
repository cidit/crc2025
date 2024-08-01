#pragma once
#include "sensor.hpp"
#include <Arduino.h>
#include "util.hpp"
#include <Encoder.h>

struct RotaryEncoderData
{
    Direction direction;
    uint32_t speed; // rpm
    float angle;    // radians
};

class RotaryEncoder : Sensor<RotaryEncoderData>
{
public:
    Encoder _internal_encoder;
    float _ticks_per_rotation;

    RotaryEncoder(pin_t clock, pin_t clock_offset, float ticks_per_rotation)
        : _internal_encoder(Encoder(clock, clock_offset)),
          _ticks_per_rotation(ticks_per_rotation) {}
    RotaryEncoder(Encoder internal_encoder, float ticks_per_rotations)
        : _internal_encoder(internal_encoder),
          _ticks_per_rotation(ticks_per_rotations) {}

    void begin() override;
    bool sample(RotaryEncoderData &out) override;
};
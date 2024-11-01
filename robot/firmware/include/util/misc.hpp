#pragma once
#include <Arduino.h>

#define pin_t uint8_t

enum class Direction: bool
{
    CLOCKWISE,
    COUNTER_CLOCKWISE,
};

const uint32_t ONE_SECOND = 1000;


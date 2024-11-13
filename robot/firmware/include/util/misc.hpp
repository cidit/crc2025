#pragma once
#include <Arduino.h>

#define pin_t uint8_t

enum class Direction: bool
{
    CLOCKWISE,
    COUNTER_CLOCKWISE,
};

const uint32_t ONE_SECOND = 1000;

#ifdef PLTFRM_ESP32
constexpr uint16_t HALF_PWM_OUTPUT = 65536/2;
#else
constexpr uint8_t HALF_PWM_OUTPUT = 128;
#endif
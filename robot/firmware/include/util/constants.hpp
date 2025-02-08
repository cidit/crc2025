#pragma once

constexpr uint32_t ONE_SECOND = 1000;
constexpr double EPSILON = 0.000001; // used as tolerance in floating point math


#ifdef PLTFRM_ESP32
constexpr uint16_t HALF_PWM_OUTPUT = 65536 / 2;
#else
constexpr uint8_t HALF_PWM_OUTPUT = 127;
#endif


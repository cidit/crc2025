#pragma once

const uint32_t ONE_SECOND = 1000;


#ifdef PLTFRM_ESP32
constexpr uint16_t HALF_PWM_OUTPUT = 65536 / 2;
#else
constexpr uint8_t HALF_PWM_OUTPUT = 127;
#endif


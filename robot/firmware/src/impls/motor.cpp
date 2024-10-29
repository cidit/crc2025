#if defined(USING_CRCLIB)
#include "CrcLib.h"
#endif
#include "drives/motor.hpp"

void drives::Motor::begin()
{
#ifdef USING_CRCLIB
    // when running on the CRC lib we only need one pin for all of the signalization.
    CrcLib::InitializePwmOutput(_pins.clockwise_pin);
#elif PIOENV == esp32testbench
    pinMode(_pins.speed_pin, OUTPUT);
#endif
}

void drives::Motor::set_speed(double speed)
{
    auto direction_adjusted_speed = _is_inverted ? -speed : speed;
    auto constrained_speed = constrain(direction_adjusted_speed, -1.0, 1.0);
    auto pwm_output = constrained_speed * HALF_PWM_OUTPUT + HALF_PWM_OUTPUT;
#if defined(USING_CRCLIB)
    CrcLib::SetPwmOutput(_pins.speed_pin, constrained_speed * 255);
#elif PIOENV == esp32testbench
    analogWrite(_pins.speed_pin, pwm_output);
#endif
}

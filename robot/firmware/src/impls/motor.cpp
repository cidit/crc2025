#if defined(USING_CRCLIB)
#include "CrcLib.h"
#endif
#include "drives/motor.hpp"

void drives::Motor::begin()
{
#ifdef USING_CRCLIB
    // when running on the CRC lib we only need one pin for all of the signalization.
    CrcLib::InitializePwmOutput(_pins.clockwise_pin);
#endif
}

void drives::Motor::set_speed(double speed)
{
    auto direction_adjusted_speed = _is_inverted ? -speed : speed;
    auto constrained_speed = constrain(direction_adjusted_speed, -1.0, 1.0);
#if defined(USING_CRCLIB)
    CrcLib::SetPwmOutput(_pins.speed_pin, constrained_speed * HALF_PWM_OUTPUT);
#endif
}

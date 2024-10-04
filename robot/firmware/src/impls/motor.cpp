#include "CrcLib.h"
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
#ifdef USING_CRCLIB
    double real_speed = constrain(_is_inverted ? -speed : speed, -1.0, 1.0);
    CrcLib::SetPwmOutput(_pins.clockwise_pin, real_speed * 255);
#endif
}

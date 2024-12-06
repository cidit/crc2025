#if defined(USING_CRCLIB)
#include "CrcLib.h"
#endif
#include "drives/motor.hpp"
#include <limits.h>

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
    auto actual_speed = constrained_speed > 0
                            ? constrained_speed * CHAR_MAX
                            : constrained_speed * (CHAR_MAX + 1);
    _cached_real_speed = actual_speed;
#endif
}

void drives::Motor::loop() {
    CrcLib::SetPwmOutput(_pins.clockwise_pin, _cached_real_speed);
}


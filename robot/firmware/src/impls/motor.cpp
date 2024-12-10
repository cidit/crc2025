#if defined(USING_CRCLIB)
#include "CrcLib.h"
#endif
#include "drives/motor.hpp"

//------- CONSTANTS -------
#define MAX_SPEED 0x7f //127
//-------------------------

//---------------------- CONSTRUCTORS ---------------------------
drives::Motor::Motor(MotorPins_t pins,bool inverted = false)
{
    _pins = pins;
    _is_inverted = inverted;
}

/**
 * Must be called in Setup
 * Initialize basic things like Pins
 */
void drives::Motor::begin()
{
#ifdef USING_CRCLIB
    // when running on the CRC lib we only need one pin for all of the signalization.
    CrcLib::InitializePwmOutput(_pins.clockwise_pin);
#endif
}


//-------------------------- PUBLICS -----------------------------
/**
 * Must be called in Loop
 * Set power of moteurs
 */
void drives::Motor::loop() {
    CrcLib::SetPwmOutput(_pins.clockwise_pin, _cached_real_speed);
}

/**
 * sets the speed.
 * @param speed a double between -1 and 1. will immediatly modify the speed of the motor.
 */
void drives::Motor::set_speed(double speed)
{
    //Apply sign and contrain
    auto direction_adjusted_speed = _is_inverted ? -speed : speed;
    auto constrained_speed = constrain(direction_adjusted_speed, -1.0, 1.0);

#if defined(USING_CRCLIB)
    //Multiply the speed ratio by the max value
    //+1 if positive because max of 8 bits is 128
    auto actual_speed = constrained_speed > 0
                            ? constrained_speed * MAX_SPEED
                            : constrained_speed * (MAX_SPEED + 1);
    _cached_real_speed = actual_speed;
#endif
}


#pragma once
#include "util_defs.hpp"

enum MotorDirection {
  CLOCKWISE, COUNTER_CLOCKWISE, NONE
};

typedef struct MotorPins_s {
    pin_t clockwise_pin, counter_clockwise_pin, speed_pin;
} MotorPins_t;

class Motor {
    MotorPins_t _pins;
    bool _is_inverted;
public:
    Motor(
        MotorPins_t
    );
    Motor(
        MotorPins_t,
        bool inverted
    );

    /**
     * sets the speed.
     * @param speed an unsigned integer between `std::numeric_limits<uint16_t>::min()` and `std::numeric_limits<uint16_t>::max()`. will immediatly modify the speed of the motor.
    */
    virtual void set_speed(uint16_t);
};

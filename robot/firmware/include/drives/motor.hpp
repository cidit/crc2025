#pragma once
#include "util/misc.hpp"

namespace drives
{

    typedef struct MotorPins_s
    {
        pin_t clockwise_pin, counter_clockwise_pin, speed_pin;
    } MotorPins_t;

    class Motor
    {
        MotorPins_t _pins;
        bool _is_inverted;

    public:
        Motor(MotorPins_t pins,
              bool inverted = false)
            : _pins(pins),
              _is_inverted(inverted) {}

        void begin();

        /**
         * sets the speed.
         * @param speed a double between -1 and 1. will immediatly modify the speed of the motor.
         */
        void set_speed(double);
    };

}
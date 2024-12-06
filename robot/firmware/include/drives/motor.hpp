#pragma once
#include "util/misc.hpp"
#include "util/looped.hpp"

namespace drives
{

    

    class Motor : public Looped
    {
        

    public:
        typedef struct MotorPins_s
        {
            pin_t clockwise_pin, counter_clockwise_pin, speed_pin;
        } MotorPins_t;

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

        void loop() override;
    private:
        MotorPins_t _pins;
        bool _is_inverted;
        char _cached_real_speed;
    };
}
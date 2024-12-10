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

        Motor(MotorPins_t pins, bool inverted = false);
        void begin();
        void loop() override;
        void set_speed(double);

    private:
        MotorPins_t _pins;
        bool _is_inverted;
        char _cached_real_speed;
    };
}
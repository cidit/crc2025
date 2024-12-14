#pragma once
#include "util/misc.hpp"
#include "util/looped.hpp"

namespace drives
{
    class Motor
    {
    public:
        //-------------------------- CONST -----------------------------
        const int MAX_SPEED = 127;

        //---------------------- CONSTRUCTORS ---------------------------
        Motor(int pin, bool inverted = false):
            _pin(pin),
            _is_inverted(inverted)
        {
            //no-op
        }

        //-------------------------- PUBLICS -----------------------------
        /**
         * Must be called in Setup
         * Initialize basic things like Pins
         */
        void begin(){
            CrcLib::InitializePwmOutput(_pin);
        }

        /**
         * sets the speed.
         * @param ratio a double between -1 and 1. will immediatly modify the speed of the motor.
         */
        void set_power_ratio(double ratio){
            //Apply sign and contrain
            auto direction_adjusted_speed = _is_inverted ? -ratio : ratio;
            auto constrained_speed = constrain(direction_adjusted_speed, -1.0, 1.0);

            //Multiply the speed ratio by the max value
            //-1 if positive because the value is between -128 and 127
            auto actual_speed = constrained_speed > 0
                                    ? constrained_speed * (MAX_SPEED - 1)
                                    : constrained_speed * MAX_SPEED ;
            _cached_real_speed = actual_speed;
            set_power(_cached_real_speed);
        }

        /**
         * Sets the speed.
         * @param power a double between -128 and 127. will immediatly modify the speed of the motor.
         */
        void set_power(double power){
            power = constrain(power, -128.0, 127.0);
            CrcLib::SetPwmOutput(_pin, power);
        }

    private:
        int _pin;
        bool _is_inverted;
        char _cached_real_speed;
    };
}
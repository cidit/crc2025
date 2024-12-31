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
        double _last_power = 0;

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
            Serial.println(" > Init Motor" + String(_pin));
            CrcLib::InitializePwmOutput(_pin);
        }

        /**
         * sets the power.
         * @param power a double between -1 and 1. will immediatly modify the power of the motor.
         */
        void set_power_ratio(double power){
            auto constrained_power = constrain(power, -1.0, 1.0);
            _last_power = constrained_power;

            // Multiply the power ratio by the max value PWM value.
            set_power(constrained_power * MAX_SPEED);
        }

        /**
         * Sets the speed.
         * @param power a double between -128 and 127. will immediatly modify the speed of the motor.
         */
        void set_power(double power){
            power = constrain(power, -128.0, 127.0);
            
            //Change sign if inverted
            power = _is_inverted ? -power : power;

            CrcLib::SetPwmOutput(_pin, power);
        }

    private:
        int _pin;
        bool _is_inverted;
    };
}
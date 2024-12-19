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
            Serial.println(" > Init Motor");
            CrcLib::InitializePwmOutput(_pin);
        }

        /**
         * sets the speed.
         * @param ratio a double between -1 and 1. will immediatly modify the speed of the motor.
         */
        void set_power_ratio(double ratio){
            auto constrained_speed = constrain(ratio, -1.0, 1.0);

            //Multiply the speed ratio by the max value
            set_power(constrained_speed * MAX_SPEED);
        }

        /**
         * Sets the speed.
         * @param power a double between -128 and 127. will immediatly modify the speed of the motor.
         */
        void set_power(double power){
            power = constrain(power, -128.0, 127.0);
            
            //Change sign if inverted
            power = _is_inverted ? -power : power;

            Serial.print(" Power: "+String(power)+"\n");
            CrcLib::SetPwmOutput(_pin, power);
        }

    private:
        int _pin;
        bool _is_inverted;
    };
}
#pragma once
#include "util/misc.hpp"
#include "util/looped.hpp"

class Motor
{
public:
    //-------------------------- CONST -----------------------------
    const int MAX_SPEED = 127;
    double _last_power = 0;

    //---------------------- CONSTRUCTORS ---------------------------
    Motor(int pin, bool inverted = false) : _pin(pin),
                                            _is_inverted(inverted)
    {
        // no-op
    }

    //-------------------------- PUBLICS -----------------------------
    /**
     * Must be called in Setup
     * Initialize basic things like Pins
     */
    void begin()
    {
        Serial.println(" > Init Motor" + String(_pin));
        CrcLib::InitializePwmOutput(_pin);
    }

    /**
     * sets the power.
     * @param power a double between -1 and 1. will immediatly modify the power of the motor.
     */
    void set_power_ratio(double power)
    {
        auto constrained_power = constrain(power, -1.0, 1.0);
        _last_power = constrained_power;
        auto pwm = constrained_power * HALF_PWM_OUTPUT;
        CrcLib::SetPwmOutput(_pin, _is_inverted ? -pwm : pwm);
    }

    /**
     * @deprecated kept around for backward compatibility. use `set_power_ratio`
     * // TODO: once this function gets removed, rename `set_power_ratio` to `set_power`
     *
     *
     * Sets the speed.
     * @param power a double between -128 and 127. will immediatly modify the speed of the motor.
     */
    void set_power(double power)
    {
        power = constrain(power, -128.0, 127.0);

        // Change sign if inverted
        power = _is_inverted ? -power : power;

        CrcLib::SetPwmOutput(_pin, power);
    }

    double get_power() {
        return _last_power;
    }

private:
    int _pin;
    bool _is_inverted;
};
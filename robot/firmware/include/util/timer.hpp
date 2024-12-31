#pragma once
#include <Arduino.h>

using timestamp = uint32_t;

class Timer
{
    timestamp _last = 0;

public:

    /**
     * this delay is in milliseconds
     */
    timestamp _delay;

    Timer(timestamp delay) : _delay(delay) {}

    bool is_time(timestamp now)
    {
        if (now - _last < _delay)
        {
            return false;
        }
        _last = now;
        return true;
    }
};
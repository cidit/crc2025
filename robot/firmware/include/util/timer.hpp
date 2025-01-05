#pragma once
#include <Arduino.h>

using timestamp = uint32_t;

class Timer
{

public:

    /**
     * milliseconds
     */
    timestamp _delay, _last, _now;


    /**
     * @param delay the delay in milliseconds.
     * @param now the initial time in milliseconds. give it a time bigger than the initial time to delay the first execution of the timer.
     */
    Timer(timestamp delay, timestamp now = 0) 
    : _delay(delay), _last(0), _now(now) {}

    bool is_time()
    {
        return _now - _last > _delay;
    }

    void update_now(timestamp now) {
        if (is_time()) {
            // before we update the current now, we must update last if it was triggered last time.
            _last = _now;
        }
        if (_now < now) {
            _now = now;
        }
    }
};
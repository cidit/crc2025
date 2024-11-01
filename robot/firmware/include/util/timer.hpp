#pragma once
#include <Arduino.h>

using timestamp = uint32_t;

class Timer {
    timestamp _last=0;

public:
    const timestamp _delay;
    Timer(timestamp delay): _delay(delay) {}

    bool time(timestamp now) {
        if (now -_last > _delay) {
            _last = now;
            return true;
        }
        return false;
    }
};
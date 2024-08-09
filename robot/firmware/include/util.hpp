#pragma once
#include <Arduino.h>

#define pin_t uint8_t

enum Direction
{
    CLOCKWISE,
    COUNTER_CLOCKWISE
};

class Looped
{
public:
    virtual ~Looped() = default;

    /**
     * represents a loop step.
     */
    virtual void loop() = 0;
};
#pragma once

#include <sensors/sensor.hpp>
#include <util/timer.hpp>

/**
 * pretends to be a linear encoder (like Paul Strauf's) but actually just reads a value from somewhere else.
 *
 */
class LinEncSpoof : public Sensor<int32_t>
{
public:
    const int32_t &_actual;

    LinEncSpoof(
        const int32_t &actual_value_to_read,
        Timer &polling_timer) 
        : Sensor(0, polling_timer), _actual(actual_value_to_read) {}

    void begin() override { /* no-op*/ };
    bool sample(int32_t &out)
    {
        out = _actual;
    }
};
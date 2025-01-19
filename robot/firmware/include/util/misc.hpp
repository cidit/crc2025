#pragma once
#include <Arduino.h>
#include <PID_RT.h>

#define pin_t uint8_t

enum class Direction : bool
{
    CLOCKWISE,
    COUNTER_CLOCKWISE,
};

const uint32_t ONE_SECOND = 1000;

#ifdef PLTFRM_ESP32
constexpr uint16_t HALF_PWM_OUTPUT = 65536 / 2;
#else
constexpr uint8_t HALF_PWM_OUTPUT = 127;
#endif

struct PIDTunings
{
    float p, i, d;
};

/**
 * returns the pid to a sane state by resetting the errorsum, lasttime and derivative factors.
 */
void pid_soft_reset(PID_RT &pid)
{
    // 1. save the parameters we want to re-establish
    auto k = (PIDTunings){
        .p = pid.getKp(),
        .i = pid.getKi(),
        .d = pid.getKd(),
    };
    auto interval = pid.getInterval();
    auto out_min = pid.getOutputMin();
    auto out_max = pid.getOutputMax();
    auto reversed = pid.getReverse();
    auto poe = pid.isPropOnError();

    // 2. do the complete reset
    pid.reset();

    // 3. restore the parameters we saved
    pid.setK(k.p, k.i, k.d);
    pid.setInterval(interval);
    pid.setOutputRange(out_min, out_max);
    pid.setReverse(reversed);
    if (poe)
    {
        pid.setPropOnError();
    }
    else
    {
        // poi is the default so we dont need to do anything.
    }
}
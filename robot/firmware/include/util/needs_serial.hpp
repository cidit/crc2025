#pragma once
#include <Arduino.h>

namespace NeedsSerial
{
    int serialAvailable = false;

    void wait_for_serial(uint32_t timeout_ms = 1000)
    {
        auto start = millis();
        while (!Serial)
        {
            if (millis() - start > timeout_ms)
            {
                return;
            }
        }
        serialAvailable = true;
        return;
    }

#define NS(ns)                            \
    {                                     \
        if (NeedsSerial::serialAvailable) \
        {                                 \
            ns;                           \
        }                                 \
    };

}
#pragma once
#include <Arduino.h>

namespace NeedsSerial
{
    int serialAvailable = false;

#define NS(ns) {                        \
        if (NeedsSerial::serialAvailable) \
        {                    \
            ns;              \
        }                    \
    };

}
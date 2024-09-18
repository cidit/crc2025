#pragma once
#include <Arduino.h>

namespace NeedsSerial
{
    int serialAvailable = false;

#define NS(ns)               \
    {                        \
        if (NeedsSerial::serialAvailable) \
        {                    \
            ns;              \
        }                    \
    }

}

int main(int argc, char const *argv[])
{

    NeedsSerial::serialAvailable = !!Serial;

    NeedsSerial::NS({
        Serial.print("balh");
        Serial.println();
        })


    return 0;
}

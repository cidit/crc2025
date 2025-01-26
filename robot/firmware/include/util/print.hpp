#pragma once

#include <Arduino.h>

#define SPRINT(things) Serial.print(things)
#define SEPARATOR Serial.print(" | ")

#ifndef ARDUINO_SAM_DUE

#include <PID_RT.h>
void print_pid_vals(PID_RT& pid)
{
    Serial.println("Kp: " +
                   String(pid.getKp(), 5) +
                   " Ki: " +
                   String(pid.getKi(), 5) +
                   " Kd: " +
                   String(pid.getKd(), 5));
}

#endif

// https://github.com/ElvisKremmen/Arduino-Code-Fragments/blob/main/Numeric%20string%20left-pad%20function
String padLeft(String inString, uint16_t newLen, char c)
{ // Pad a numeric string with spaces for output
    while (inString.length() < newLen)
    {
        inString = String(c) + inString;
    };
    return inString;
}


String padRight(String inString, uint16_t newLen, char c)
{ // Pad a numeric string with spaces for output
    while (inString.length() < newLen)
    {
        inString = inString + String(c);
    };
    return inString;
}
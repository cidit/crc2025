#pragma once

#include <Arduino.h>
#include <PID_RT.h>

#define SPRINT(things) Serial.print(things)
#define SPACER Serial.print("    ")

void print_pid_vals(PID_RT& pid)
{
    Serial.println("Kp: " +
                   String(pid.getKp(), 5) +
                   " Ki: " +
                   String(pid.getKi(), 5) +
                   " Kd: " +
                   String(pid.getKd(), 5));
}


// https://github.com/ElvisKremmen/Arduino-Code-Fragments/blob/main/Numeric%20string%20left-pad%20function
String padLeft(String inString, uint16_t newLen)
{ // Pad a numeric string with spaces for output
    while (inString.length() < newLen)
    {
        inString = String(" ") + inString;
    };
    return inString;
}


String padRight(String inString, uint16_t newLen)
{ // Pad a numeric string with spaces for output
    while (inString.length() < newLen)
    {
        inString = inString + String(" ");
    };
    return inString;
}
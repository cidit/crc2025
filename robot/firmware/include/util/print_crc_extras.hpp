#include "util/print.hpp"


#include <PID_RT.h>
#include <CrcLib.h>



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

void print_pid_vals(PID_RT &pid)
{
    Serial.println("Kp: " +
                   String(pid.getKp(), 5) +
                   " Ki: " +
                   String(pid.getKi(), 5) +
                   " Kd: " +
                   String(pid.getKd(), 5));
}

void print_battery()
{
    // https://www.batteryskills.com/12-volt-battery-voltage-chart/
    constexpr double CHARGE_0_PERCENT = 11.8,
                     CHARGE_100_PERCENT = 12.8,
                     CHARGE_USABLE_RANGE = CHARGE_100_PERCENT - CHARGE_0_PERCENT,
                     CHARGE_DANGEROUS_LEVEL = .40; // 40%

    static bool blink_seen = true;
    auto volatge = CrcLib::GetBatteryVoltage();
    auto voltage_percent = (volatge - CHARGE_0_PERCENT) / CHARGE_USABLE_RANGE;
    if (voltage_percent < 0) {
        SPRINT("!!!!");
        return;
    }

    String displayable_voltage = padLeft(String(int(voltage_percent * 100)), 3, ' ') + '%';

    auto is_safe_level = voltage_percent > CHARGE_DANGEROUS_LEVEL;
    if (is_safe_level || blink_seen)
    {
        SPRINT(displayable_voltage);
    }
    else
    {
        SPRINT("!!!!");
    }

    blink_seen = !blink_seen;
}

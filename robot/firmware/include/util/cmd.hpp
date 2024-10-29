#pragma once

#include <Arduino.h>

enum class Command : int
{
  // arg: angle in degrees (0-360)
  AIM,
  // arg: rpm
  RPM,
  // arg: speed to give to the motor, between -1 and 1
  SPD,

  UNKNOWN
};

Command cmd_from_string(String c)
{
#define case(name)                 \
  {                                \
    if (c.equalsIgnoreCase(#name)) \
      return Command::name;        \
  }

  case(AIM)
  case(RPM)
  case(SPD)
  return Command::UNKNOWN;
}
#pragma once

#include <Arduino.h>

enum Command : int
{
  AIM,
  UNKNOWN
};

int cmd_from_string(String c)
{
  if (c.equalsIgnoreCase("AIM"))
    return Command::AIM;
  return Command::UNKNOWN;
}
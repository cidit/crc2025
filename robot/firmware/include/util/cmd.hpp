#pragma once

#include <Arduino.h>

enum class Command : int
{
  AIM,
  UNKNOWN
};

Command cmd_from_string(String c)
{
  if (c.equalsIgnoreCase("AIM"))
    return Command::AIM;
  return Command::UNKNOWN;
}
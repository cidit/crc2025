#include <Arduino.h>
#include <Decodeur.h>
#include <CrcLib.h>
#include "drives/precision_motor.hpp"
#include "drives/motor.hpp"
#include "sensors/rotary_encoder.hpp"

Decodeur cmdl(&Serial);
drives::Motor motor({CRC_PWM_1, 0, 0}, false);
sensors::LinearEncoder le(CRC_ENCO_A, CRC_ENCO_B);
sensors::RotaryEncoder re(le, 537.4);
float motor_speed;

void setup()
{
  Serial.begin(115200);
  CrcLib::Initialize();
  motor.begin();
  le.begin();
  re.begin();
}

bool print_timer(long now)
{
  const long length = 500; // ms
  static long last = 0;    // ms
  if (now - last > length)
  {
    last = now;
    return true;
  }
  return false;
}

void loop()
{
  cmdl.refresh();
  CrcLib::Update();
  // le.poll();
  re.poll();
  motor.set_speed(motor_speed);

  if (print_timer(millis()))
  {
    // Serial.println(re.getLast()._radians);
    Serial.println(le.getLast());
  }

  if (!cmdl.isAvailable())
  {
    return;
  }
  auto ack = true;
  switch (cmdl.getCommand())
  {
  case 'S':
  {
    if (cmdl.getArgCount() < 1)
    {
      ack = false;
      break;
    }
    auto speed = cmdl.getArg(0);
    Serial.println("setting speed to: " + String(speed));
    motor_speed = speed;
    break;
  }
  default:
  {
    ack = false;
    break;
  }
  }
  Serial.println(ack ? "!" : "?");
}
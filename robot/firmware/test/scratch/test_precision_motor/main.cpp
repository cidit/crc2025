#include <Arduino.h>
#include <Decodeur.h>
#include <CrcLib.h>
#include <unity.h>
#include "drives/precision_motor.hpp"
#include "drives/motor.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"

Decodeur cmdl(&Serial);
Motor motor({CRC_PWM_1, 0, 0}, false);
sensors::GobuildaRotaryEnco re(CRC_ENCO_A, CRC_ENCO_B, 538.4);
float motor_speed;


void test_led_builtin_pin_number(void)
{
  TEST_ASSERT_EQUAL(true, true);
}


void setup()
{
  Serial.begin(115200);
  CrcLib::Initialize();
  motor.begin();
  re.begin();
  UNITY_BEGIN(); // IMPORTANT LINE!
}

Timer print_timer(500); // ms

void loop()
{
  cmdl.refresh();
  CrcLib::Update();
  // le.poll();
  re.poll();
  motor.set_speed(motor_speed);

  if (print_timer.is_time(millis()))
  {
    // Serial.println(re.getLast()._radians);
    Serial.println(re.getLast());
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
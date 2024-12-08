#include <Arduino.h>
#include <Decodeur.h>
#include <CrcLib.h>
#include <PID_v1.h>
#include "drives/precision_motor.hpp"
#include "drives/motor.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"


Decodeur cmdl(&Serial);
drives::Motor motor({CRC_PWM_1, 0, 0}, false);
sensors::GobuildaRotaryEnco re(CRC_ENCO_A, CRC_ENCO_B, 538.4);
double in, out, s=0;
PID pid(&in, &out, &s, 0,0,0, DIRECT);
auto target_angle = math::Angle::zero();

void setup()
{
  Serial.begin(115200);
  CrcLib::Initialize();
  motor.begin();
  re.begin();
  pid.SetSampleTime(1);
  pid.SetOutputLimits(-1, 1);
  pid.SetMode(AUTOMATIC);
}

Timer print_timer(500); // ms

void loop()
{
  cmdl.refresh();
  CrcLib::Update();
  re.poll();
  motor.loop();

  in = math::Angle::travel(re.getLast(), target_angle);
  if (pid.Compute()) {
    motor.set_speed(out);
  }

  if (print_timer.is_time(millis()))
  {
    Serial.println("a:"+ String(re.getLast()._radians) + " i:" + String(in) + " o:" + String(out));
  }

  if (!cmdl.isAvailable())
  {
    return;
  }

  auto ack = true;
  switch (toupper(cmdl.getCommand()))
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
    motor.set_speed(speed);
    break;
  }
  case 'K': {
    if (cmdl.getArgCount() < 3) {
      ack = false;
      break;
    }
    double p = cmdl.getArg(0), i = cmdl.getArg(1), d=cmdl.getArg(2);
    pid.SetTunings(p, i, d);
    break;
  }
  case 'A': {
    if (cmdl.getArgCount()<1) {
      ack = false;
      break;
    }
    target_angle = math::Angle::from_rad(cmdl.getArg(0));
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
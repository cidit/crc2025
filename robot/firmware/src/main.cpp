#include <Arduino.h>
#include <Decodeur.h>
#include <CrcLib.h>
#include <PID_RT.h>
#include "drives/precision_motor.hpp"
#include "drives/motor.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "Encoder.h"
#include "math/angles.hpp"

//-------------------------- DEFINES -----------------------------



//-------------------------- VARIABLES -----------------------------
double in, out, s=0;
auto target_angle = math::Angle::zero();


//-------------------------- OBJECTS -----------------------------
Decodeur cmdl(&Serial);

drives::Motor motor(CRC_PWM_1, false);
Encoder enco(CRC_ENCO_A, CRC_ENCO_B);
drives::PrecisionMotor pm(motor, enco, 1.0, 0.6, 0.0, 0.08, 0.08, 0.0002, 10, 312.0, drives::PrecisionMotor::TICKS_312);

//-------------------------- FUNCTIONS -----------------------------
void update_cmd(){
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
    Serial.print("Speed: ");
    Serial.println(speed);
    pm.set_target_speed(speed);
    break;
  }
  case 'K': {
    if (cmdl.getArgCount() < 3) {
      ack = false;
      break;
    }
    double p = cmdl.getArg(0), i = cmdl.getArg(1), d=cmdl.getArg(2);
    Serial.print("Tunings: " + String(p) + " " + String(i) + " " + String(d));
    pm.set_speed_pid(p, i, d);
    break;
  }
  case 'A': {
    if (cmdl.getArgCount()<1) {
      ack = false;
      break;
    }
    target_angle = math::Angle::from_rad(cmdl.getArg(0));
    Serial.print("Rads: ");
    Serial.println(target_angle._radians);
    pm.set_target_angle(target_angle);
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


//-------------------------- MAIN PROG -----------------------------
void setup()
{
  Serial.begin(115200);
  CrcLib::Initialize();
  motor.begin();
}

void loop()
{
  cmdl.refresh();
  update_cmd();

  CrcLib::Update();
  pm.loop();

}



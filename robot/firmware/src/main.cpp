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

Encoder enco1(CRC_I2C_SDA, CRC_DIG_2), 
        enco2(CRC_I2C_SCL, CRC_DIG_3),
        enco3(CRC_ENCO_A, CRC_DIG_4),
        enco4(CRC_ENCO_B, CRC_DIG_5);

//-------------------------- FUNCTIONS -----------------------------
void update_cmd(){
  if (!cmdl.isAvailable())
  {
    return;
  }

  auto ack = true;
  switch (toupper(cmdl.getCommand()))
  {
  // case 'S':
  // {
  //   if (cmdl.getArgCount() < 1)
  //   {
  //     ack = false;
  //     break;
  //   }
  //   auto speed = cmdl.getArg(0);
  //   Serial.println("setting speed to: " + String(speed));
  //   Serial.print("Speed: ");
  //   Serial.println(speed);
  //   pm.set_target_speed(speed);
  //   break;
  // }
  // case 'K': {
  //   if (cmdl.getArgCount() < 3) {
  //     ack = false;
  //     break;
  //   }
  //   double p = cmdl.getArg(0), i = cmdl.getArg(1), d=cmdl.getArg(2);
  //   Serial.print("Tunings: " + String(p) + " " + String(i) + " " + String(d));
  //   pm.set_speed_pid(p, i, d);
  //   break;
  // }
  // case 'A': {
  //   if (cmdl.getArgCount()<1) {
  //     ack = false;
  //     break;
  //   }
  //   target_angle = math::Angle::from_rad(cmdl.getArg(0));
  //   Serial.print("Rads: ");
  //   Serial.println(target_angle._radians);
  //   pm.set_target_angle(target_angle);
  //   break;
  // }
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
}

void loop()
{
  cmdl.refresh();
  update_cmd();
  CrcLib::Update();

  char buf[150];
  sprintf(buf, "1:%d 2:%d 3:%d 4:%d", enco1.read(), enco2.read(), enco3.read(), enco4.read());
  Serial.println(buf);  
}



#include <Arduino.h>
#include <Decodeur.h>
#include <CrcLib.h>
#include <PID_RT.h>
#include <Encoder.h>
#include "drives/precision_motor.hpp"
#include "drives/motor.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "math/angles.hpp"

//-------------------------- DEFINES -----------------------------

#define ENCODER_DO_NOT_USE_INTERRUPTS

//-------------------------- VARIABLES -----------------------------


//-------------------------- OBJECTS -----------------------------

Decodeur cmdl(&Serial);

// Encoder enco1(CRC_I2C_SDA, CRC_DIG_2), 
//         enco2(CRC_I2C_SCL, CRC_DIG_3),
//         enco3(CRC_ENCO_A, CRC_DIG_4),
//         enco4(CRC_ENCO_B, CRC_DIG_5);
        
Encoder enco1(CRC_I2C_SDA, CRC_DIG_4), 
        enco2(CRC_I2C_SCL, CRC_DIG_5),
        enco3(CRC_ENCO_A, CRC_DIG_2),
        enco4(CRC_ENCO_B, CRC_DIG_3);

const int pins[8] = {
  CRC_I2C_SDA, CRC_I2C_SCL, CRC_ENCO_A, CRC_ENCO_B, CRC_DIG_4, CRC_DIG_5, CRC_DIG_2, CRC_DIG_3
};

//-------------------------- MAIN PROG -----------------------------

void setup()
{
  Serial.begin(115200);
  CrcLib::Initialize();
}

void loop()
{
  cmdl.refresh();
  CrcLib::Update();

  char buf[150];
  for (auto p: pins) {
    Serial.println(p);
    Serial.print(":");
    Serial.print(digitalRead(p));
    Serial.print(" ");
  }
  Serial.println();
  sprintf(buf, "1:%d 2:%d 3:%d 4:%d", enco1.read(), enco2.read(), enco3.read(), enco4.read());
  Serial.println(buf);  

}

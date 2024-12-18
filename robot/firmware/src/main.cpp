/*
  Main program for robot control

    
*/
#include <CrcLib.h>
#include <Encoder.h>
#include <drives/swerve_module.hpp>
#include <controller.hpp>
#include "util/looped.hpp"
#include "math/vectors.hpp"
using math::cartesian::Vec2D;

//----- Variables -----
Controller ctrl;

//SWERVE A
drives::Motor motorAH(CRC_PWM_4);
Encoder encoAH(CRC_ENCO_A, CRC_ENCO_B);
drives::PrecisionMotor pmAH(motorAH, encoAH, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
drives::Motor motorAB(CRC_PWM_3);
Encoder encoAB(CRC_I2C_SCL, CRC_I2C_SDA);
drives::PrecisionMotor pmAB(motorAB, encoAB, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
SwerveModule swerveA(pmAH, pmAB, CRC_PWM_12);
PID_RT pidSwerveA;


Looped *loopable[] = {
  &swerveA,
  &pmAH,
  &pmAB
};

//----- Main Program ----------------------------------------------------------------------
void setup(){
  Serial.begin(115200);

  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  swerveA.begin();
  
  Serial.println("Setup Done");
}

void loop(){
  CrcLib::Update();
  ctrl.update();
  //Serial.println(ctrl.get_left_joy().angleRad, ctrl.get_left_joy().norm);
  swerveA.set_target(ctrl.get_left_joy().angleRad , ctrl.get_left_joy().norm);

  //Loop through all loopable
  for(auto item:loopable){
    item->loop();
  }
  Serial.println();
}


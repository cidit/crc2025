/*
  Main program for robot control


  SwerveAH : PWM_4
  SwerveAB : PWM_3
  SwerveBH : PWM_7
  SwerveBB : PWM_1
  PincePoignet : PWM_2
  BrasA : PWM_6
  BrasB : PWM_5

  Pour encodeur: Blanc=A, Jaune=B
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

//----- Bras ------
// drives::Motor motorO(CRC_PWM_2);
// Encoder encoO(CRC_I2C_SDA, CRC_I2C_SCL);
// drives::PrecisionMotor pmO(motorO, encoO, 10, 117.0, drives::PrecisionMotor::TICKS_117);

//----- SWERVE A -----
// drives::Motor motorAH(CRC_PWM_4);
// Encoder encoAH(CRC_ENCO_A, CRC_ENCO_B);
// drives::PrecisionMotor pmAH(motorAH, encoAH, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
// drives::Motor motorAB(CRC_PWM_3);
// Encoder encoAB(CRC_I2C_SDA, CRC_I2C_SCL);
// drives::PrecisionMotor pmAB(motorAB, encoAB, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
// SwerveModule swerveA(pmAH, pmAB, CRC_PWM_12);
// PID_RT pidSwerveA;

//----- SWERVE B -----
drives::Motor motorBH(CRC_PWM_7);
Encoder encoBH(CRC_ENCO_A, CRC_ENCO_B);
drives::PrecisionMotor pmBH(motorBH, encoBH, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
drives::Motor motorBB(CRC_PWM_1);
Encoder encoBB(CRC_I2C_SDA, CRC_I2C_SCL);
drives::PrecisionMotor pmBB(motorBB, encoBB, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
SwerveModule swerveB(pmBH, pmBB, CRC_DIG_1);
// PID_RT pidSwerveB;


Looped *loopable[] = {
  // &swerveA,
  &swerveB,
  // &pmAH,
  // &pmAB,
  &pmBH,
  &pmBB
};

//----- Main Program ----------------------------------------------------------------------
void setup(){
  Serial.begin(115200);

  // Initialisation des Moteurs
  CrcLib::Initialize();
  //CrcLib::InitializePwmOutput(CRC_PWM_2);
  //CrcLib::InitializePwmOutput(CRC_PWM_1);
  //CrcLib::in

  //swerveA.begin();
  swerveB.begin();
  //pmO.begin();
  //pmO.set_target_power(100);
  //pmO.set_target_angle(math::Angle::from_rad(3.15));
  
  Serial.println("Setup Done");
}

void loop(){
  CrcLib::Update();
  //pmO.loop();

  ctrl.update();
  // //Serial.println(ctrl.get_left_joy().angleRad, ctrl.get_left_joy().norm);
  // swerveA.set_target(ctrl.get_left_joy().angleRad , ctrl.get_left_joy().norm);
  swerveB.set_target(ctrl.get_left_joy().angleRad , ctrl.get_left_joy().norm);

  //Loop through all loopable
  for(auto item:loopable){
    item->loop();
  }
  Serial.println();
}


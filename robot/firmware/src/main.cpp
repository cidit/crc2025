/*
  Main program for robot control


  SwerveAH : PWM_4
  SwerveAB : PWM_3
  SwerveBH : PWM_7
  SwerveBB : PWM_1
  PincePoignet : PWM_2
  BrasA : PWM_6
  BrasB : PWM_5

  Servo1: PWM_9
  Servo2: PWM_10
  Servo3: PWM_11

  Pour encodeur: Blanc=A, Jaune=B
*/
#include <CrcLib.h>
#include <Encoder.h>
#include <drives/swerve_module.hpp>
#include <controller.hpp>
#include "util/looped.hpp"
#include "math/vectors.hpp"
#include <Servo.h>
#include <Decodeur.h>
using math::cartesian::Vec2D;

// //----- Variables -----
// Controller ctrl;
// Decodeur cmd(&Serial);

// //----- SWERVE A (gauche) -----
drives::Motor motorAH(CRC_PWM_4);
// Encoder encoAH(CRC_ENCO_A, CRC_DIG_2);
// drives::PrecisionMotor pmAH(motorAH, encoAH, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
drives::Motor motorAB(CRC_PWM_3);
// Encoder encoAB(CRC_I2C_SDA, CRC_DIG_4);
// drives::PrecisionMotor pmAB(motorAB, encoAB, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
// SwerveModule swerveA(pmAH, pmAB, CRC_PWM_12);
// PID_RT pidSwerveA;

// //----- SWERVE B (droite) -----
drives::Motor motorBH(CRC_PWM_7);
// Encoder encoBH(CRC_I2C_SCL, CRC_DIG_5);
// drives::PrecisionMotor pmBH(motorBH, encoBH, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
drives::Motor motorBB(CRC_PWM_1);
// Encoder encoBB(CRC_ENCO_B, CRC_DIG_3);
// drives::PrecisionMotor pmBB(motorBB, encoBB, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
// SwerveModule swerveB(pmBH, pmBB, CRC_DIG_1);
// PID_RT pidSwerveB;

// //----- Main Program ----------------------------------------------------------------------
// void setup(){
//   Serial.begin(115200);

//   // Initialisation des Moteurs
//   CrcLib::Initialize();
//   //CrcLib::InitializePwmOutput(CRC_PWM_2);
//   //CrcLib::InitializePwmOutput(CRC_PWM_1);

//   swerveA.begin();
//   swerveB.begin();

//   Serial.println("Setup Done");
// }

// void loop(){
//   CrcLib::Update();

//   ctrl.update();
//   cmd.refresh();

//   // if(cmd.isAvailable()){
//   //   auto commande = cmd.getCommand();
//   //   switch(commande){
//   //     case 'P':
//   //         pmO.set_target_power(cmd.getArg(0));
//   //         break;
//   //   }
//   // }

//   //Serial.println(ctrl.get_left_joy().angleRad, ctrl.get_left_joy().norm);
//   swerveA.set_target(ctrl.get_left_joy().angleRad , ctrl.get_left_joy().norm);
//   swerveB.set_target(ctrl.get_left_joy().angleRad , ctrl.get_left_joy().norm);

//   swerveA.loop();
//   swerveB.loop();

//   Serial.println();

// }

void setup()
{
  Serial.begin(115200);
  CrcLib::Initialize();
  motorAB.begin();
  motorAH.begin();
  motorBB.begin();
  motorBH.begin();
}

void loop()
{
  CrcLib::Update();
  motorAB.set_power(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X));
  motorAH.set_power(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y));
  motorBB.set_power(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X));
  motorBH.set_power(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y));

  Serial.print("j1x:");
  Serial.print(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X));
  Serial.print("\tj1y:");
  Serial.print(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y));
  Serial.print("\tj2x:");
  Serial.print(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X));
  Serial.print("\tj2y:");
  Serial.print(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y));
  Serial.println();
}
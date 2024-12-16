/*
  Main program for robot control

  
*/
#include <CrcLib.h>
#include <Encoder.h>
#include <drives/swerve_module.hpp>
#include <controller.hpp>
#include "math/vectors.hpp"
using math::cartesian::Vec2D;

//----- Variables -----
Controller ctrl;
drives::Motor motorAH(CRC_PWM)
drives::PrecisionMotor()
SwerveModule swerveA();
PID_RT pidSwerveA;

bool motors_enabled = false;

//----- Main Program ----------------------------------------------------------------------
void setup(){
  Serial.begin(115200);

  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  // Pinmode pour le PWM
  pinMode(CRC_PWM_12, INPUT);

  //Init SwerveA
  pidSwerveA.setPoint(0); //Toujours le garder à 0
  pidSwerveA.setOutputRange(-1.0, 1.0); //Interval de ratio de puissance angulaire, le signe est appliquer plus tard
  pidSwerveA.setK(1.0, 0, 0); //Proportionnal, Integral, Derivative
  pidSwerveA.start();


  Serial.println("Setup Done");
}

void loop(){
  CrcLib::Update();
  ctrl.update();

  Vec2D vector = swerveA.calculate(ctrl.get_left_joy().angleRad , ctrl.get_left_joy().norm);
  swerveA.set_motor_powers(vector);
}


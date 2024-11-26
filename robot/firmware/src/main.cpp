#include <CrcLib.h>
#include <Encoder.h>
#include <swerveModule.hpp>
#include <controller.hpp>
#include "math/vectors.hpp"
using math::cartesian::Vec2D;

//----- Variables -----
double joyAngle = 0;
double joySpeed = 1;

Controller ctrl;
SwerveModule swerveA;

int printDelai = 500;
int printTimer = millis();

//----- Main Program ----------------------------------------------------------------------
void setup(){
  Serial.begin(115200);

  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  // Pinmode pour le PWM
  pinMode(CRC_PWM_12, INPUT);

  //Initialisation des modules Swerve
  swerveA.init(0.002, 0.00, 0.0);

  Serial.println("Setup Done");
}

void loop(){
  CrcLib::Update();
  ctrl.update();


  Vec2D vector = swerveA.calculate(ctrl.get_left_joy().angleDeg, ctrl.get_left_joy().norm);
  swerveA.setMotorPowers(vector);

  if (millis() - printTimer >= printDelai) {
    printTimer = millis();

    Serial.println("Angle: " + String(ctrl.get_left_joy().angleRad));
    Serial.println("Norm : " + String(ctrl.get_left_joy().norm));
    Serial.println("X    : " + String(ctrl.get_left_joy().x));
    Serial.println("Y    : " + String(ctrl.get_left_joy().y));
    Serial.println();
  }

}

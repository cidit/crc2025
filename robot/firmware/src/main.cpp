#include <CrcLib.h>
#include <Encoder.h>
#include <Decodeur.h>
#include <speedCalculation.hpp>
#include <controller.hpp>
#include "math/vectors.hpp"
using math::cartesian::Vec2D;

const int MAX_MOTEUR_POWER = 127;
const int MAX_ANGLE_ERROR = 5; //Allow 5 deg dif from target angle
const double SETPOINT = 0;

float speed = 0, max_pulse = 0, min_pulse = 0;
bool checking_1 = true;
double joyAngle = 0;
double joySpeed = 1;

Decodeur decodeur(&Serial);
Controller ctrl;

int printDelai = 500;
int printTimer = millis();
// PID_RT pid;

/**
 * DONE
 * Check and applu inputs from serial monitor
 */
void apply_cmds()
{
  if (!decodeur.isAvailable())
  {
    return;
  }
  
  bool ack = true;
  switch (decodeur.getCommand())
  {
  case 'A':
  {
    // change the angle
    auto newA = decodeur.getArg(0);
    if (newA > 180) newA -= 180;
    //target_angle = newA;
    break;
  }
  case 'I': {
    // increment
    // TODO: temporary
    speed = decodeur.getArg(0);
    break;
  }
  case 'T' : {
    // TODO: 
    checking_1 = !checking_1;
    break;
  }
  default:
    ack = false;
  }
  Serial.println(ack? '!': '?');
}


/**
 * Determine and set the power of the motors
 */
void setMotorPowers(Vec2D powerVector){
  //Set power according to ratio
  powerVector.set_y(powerVector.y() * MAX_MOTEUR_POWER);
  powerVector.set_x(powerVector.x() * MAX_MOTEUR_POWER);

  //TODO Verify this, Si on reduit un on devrait réduire l'autre de la même proportion non?
  double powerA = constrain(powerVector.x() + powerVector.y(), -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);
  double powerB = constrain(powerVector.x() - powerVector.y(), -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);

  CrcLib::SetPwmOutput(CRC_PWM_1, powerA);
  CrcLib::SetPwmOutput(CRC_PWM_2, powerB);
}

//---------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  // Pinmode pour le PWM
  pinMode(CRC_PWM_12, INPUT);

  Serial.println("Setup Done");

  SC::init();
}

void loop()
{
  decodeur.refresh();
  CrcLib::Update();
  ctrl.update();
  apply_cmds();

  

  // Vec2D vector = SC::calculate(ctrl.get_left_joy_vec().angle(), ctrl.get_left_joy_vec().norm());
  // setMotorPowers(vector);

  if (millis() - printTimer >= printDelai) {
    printTimer = millis();

    Serial.println("Angle: " + String(ctrl.get_left_joy().angleRad));
    Serial.println("Norm : " + String(ctrl.get_left_joy().norm));
    Serial.println("X    : " + String(ctrl.get_left_joy().x));
    Serial.println("Y    : " + String(ctrl.get_left_joy().y));
    Serial.println();

    // Serial.println(joyAngle);
    // Serial.println(joySpeed);

    // Serial.println("trans: " + String(vector.x()));
    // Serial.println("ang  : " + String(vector.y()));
    // Serial.println("test sin(90)" + String(sin(90*M_PI/180)));
    // Serial.println();
  }

}


//TODO : powerA = velocity+rPower
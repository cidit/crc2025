#include <CrcLib.h>
#include "swerveModule.hpp"
#include "math.h"
#include "math/vectors.hpp"
#include <PID_RT.h>
using math::cartesian::Vec2D;

//------- CONSTANTS -------
const double MAX_MOTEUR_POWER = 127;
//-------------------------

//---------------------- CONSTRUCTORS ---------------------------
/**
 * Initialise the default values for the calculations
 * Must be call in Setup()
 */
void SwerveModule::init(double Kp, double Ki, double Kd){
  _pid.setPoint(0); //Toujours le garder à 0
  //_pid.setReverse(false);   LL: non implementé dans la librairie alors on s'en fou
  _pid.setOutputRange(0.0, 1.0); //Interval de ratio de puissance angulaire, le signe est appliquer plus tard
  _pid.setK(Kp, Ki, Kd); //Proportionnal, Integral, Derivative
  _pid.start();
}

//-------------------------- PUBLICS -----------------------------
//##########################
//# Calculations Functions #
//##########################
/**
 * Calculate the power vector (translation, angular) of a Swerve module
 * and set the power of the motors accorddingly
 * @param targetAngle Desired angle of wheel in rads (0 to 2pi)
 * @param tPower Multiplication factor for translation power between -1.0 and 1.0
 * @return done - True: Operation completed succesfully, False: Problem
 */
bool SwerveModule::calculateAndApply(double targetAngle, double tPower){
  Vec2D vector = calculateRad(targetAngle, tPower);
  setMotorPowers(vector);

  //TODO: Implementer la logique false
  return true;
}

/**
 * Calculate the power vector (translation, angular) of a Swerve module
 * @param targetAngle Desired angle of wheel in rads (0 to 2pi)
 * @param tPower Multiplication factor for translation power between -1.0 and 1.0
 * @return vecPower - {x=translationComponent, y=angularComponent}
 */
Vec2D SwerveModule::calculateRad(double targetAngle, double tPower){  
  //Set diff and dir in _moveParam
  double currentAngle = getCurrentAngleRad();
  getdiffAngleRad(currentAngle, targetAngle);

  //Find the absolute components of the power vector of the entire module
  _vecPower.set_y(getAngularComponentRad(fabs(_moveParam.diff), fabs(tPower)));
  _vecPower.set_x(getTranslationComponentRad(fabs(_moveParam.diff), fabs(tPower)));

  // Serial.println("X" + String(_vecPower.x()));
  // Serial.println("Y" + String(_vecPower.y()));

  //Apply the good sign
  switch(_moveParam.dir){
    case SwerveModule::Direction::CLOCKWISE:
      _vecPower.set_y(fabs(_vecPower.y()));
      break;
    case SwerveModule::Direction::COUNTERCLOCKWISE:
      _vecPower.set_y(-fabs(_vecPower.y()));
      break;
  }

  //Determine the sign of the translation component dependind on current and target angle
  if(targetAngle < M_PI){
    //Forward
    if(currentAngle <= M_PI){
      _vecPower.set_x(fabs(_vecPower.x()));
    }else{
      _vecPower.set_x(-fabs(_vecPower.x()));
    }
    
  }else{
    //Backward
    if(currentAngle <= M_PI){
      _vecPower.set_x(-fabs(_vecPower.x()));
    }else{
      _vecPower.set_x(fabs(_vecPower.x()));
    }
  }


  return _vecPower;
}

//####################
//# Motors Functions #
//####################
/**
 * Set the power of the motors
 * - Si les valeurs de moteurs sont + et + -> tourne horaire
 * - Si les valeurs de moteurs sont - et - -> tourne anti-horaire
 * - Si les valeurs de moteurs sont + et - ->
 * - Si les valeurs de moteurs sont - et + ->
 * @param powerVector Vector2D with components between -1.0 and 1.0
 */
void SwerveModule::setMotorPowers(Vec2D powerVector){

  double powerB = powerVector.y() + powerVector.x();
  double powerA = powerVector.y() - powerVector.x();

  // Serial.println("PowerA: " + String(powerA));
  // Serial.println("PowerB: " + String(powerB));

  powerA = constrain(powerA*MAX_MOTEUR_POWER, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);
  powerB = constrain(powerB*MAX_MOTEUR_POWER, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);

  CrcLib::SetPwmOutput(CRC_PWM_1, -powerA);
  CrcLib::SetPwmOutput(CRC_PWM_2, -powerB);
}


//-------------------------- PRIVATES ----------------------------
//########################
//# Functions en radians #
//########################
/**
 * Set the diff angle and spin direction in _moveParam
 * Also determines travel direction
 * @param currentAngle Wheel angle in radians
 * @param targetAngle Desired angle i radians
 */
void SwerveModule::getdiffAngleRad(double currentAngle, double targetAngle){
  //Calculate travel angle, will be negative
  _moveParam.diff = targetAngle - currentAngle;
  if(_moveParam.diff >= 0){
    _moveParam.dir = _moveParam.diff < M_PI ? 
                      _moveParam.diff > M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE 
                      :_moveParam.diff > 3*M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE;
  }
  else if(_moveParam.diff < 0){
    _moveParam.dir = _moveParam.diff > -M_PI ? 
                      _moveParam.diff > -M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE 
                      :_moveParam.diff > -3*M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE;
  }
  
  
  // //Keep value in a cercle
  // while(_moveParam.diff > 2*M_PI){
  //   _moveParam.diff = _moveParam.diff - 2*M_PI;
  // }
  // while(_moveParam.diff < -2*M_PI){
  //   _moveParam.diff = _moveParam.diff + 2*M_PI;
  // }

}

/**
 * Get the current wheel angle from the absolute encoder
 * @return angle in rads
 */
double SwerveModule::getCurrentAngleRad(){
  //Get encoder value, make sure the value is between 0 and 4160 (Observation de Félix: L'encodeur retour parfois 8000)
  double enco = pulseIn(CRC_PWM_12, HIGH);

  //Calculate angle
  double angleAct = enco/4160.0 * (2*M_PI);
  Serial.println("enco: " + String(enco));
  return angleAct;
}

/**
 * Get the angular component using a ratio of the difference
 * between current and target angle, round to 0,001
 */
double SwerveModule::getAngularComponentRad(double diff, double speedFactor){
  //TODO : Remplace with PID
  double component = sin(diff)*speedFactor;
  return component;
}

/**
 * Get the translation component using a ratio of the difference
 * between current and target angle, round to 0,001
 */
double SwerveModule::getTranslationComponentRad(double diff, double speedFactor){
  double component = cos(diff)*speedFactor;
  return component;
}


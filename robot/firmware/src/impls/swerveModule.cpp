#include <CrcLib.h>
#include "swerveModule.hpp"
#include "math.h"
#include "math/vectors.hpp"
#include <PID_RT.h>
using math::cartesian::Vec2D;

//------- CONSTANTS -------
const int MAX_MOTEUR_POWER = 127;
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
/**
 * Calculate the power vector (translation, angular) of a Swerve module
 * @param targetAngle Desired angle of wheel in rads (0 to 2pi)
 * @param tPower Multiplication factor for translation power between -1.0 and 1.0
 * @return done - True: Operation completed succesfully, False: Problem
 */
bool SwerveModule::calculateAndApply(double targetAngle, double tPower){
  Vec2D vector = calculate(targetAngle, tPower);
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
Vec2D SwerveModule::calculate(double targetAngle, double tPower){
  //On s'assure que les valeurs passés en param sont conformes au range
  tPower = constrain(tPower, -1.0, 1.0);
  targetAngle = constrain(targetAngle, 0, 2*M_PI);
  
  //Set shortest and dir in _moveParam
  getShortestAngle(getCurrentAngle(), targetAngle);

  //Get the angular power from the PID
  if(_pid.compute(abs(_moveParam.shortest))){
    //Set the sign depending on direction
    switch(_moveParam.dir){
      case Direction::CLOCKWISE:
        _vecPower.set_y(_pid.getOutput());
        break;

      case Direction::COUNTERCLOCKWISE:
        _vecPower.set_y(-_pid.getOutput()); //Reverse Power
        break;
    }
  }

  //Set the translation power
  _vecPower.set_x(tPower);
    //TODO Scale translation power depending on angular power???


  return _vecPower;
}

/**
 * Set the power of the motors
 * @param powerVector Vector2D with components between -1.0 and 1.0
 */
void SwerveModule::setMotorPowers(Vec2D powerVector){
  //Set power according to ratio
  powerVector.set_y(powerVector.y() * MAX_MOTEUR_POWER);
  powerVector.set_x(powerVector.x() * MAX_MOTEUR_POWER);

  //TODO Verify this, Si on reduit un on devrait réduire l'autre de la même proportion non?
  double powerA = constrain(powerVector.x() + powerVector.y(), -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);
  double powerB = constrain(powerVector.x() - powerVector.y(), -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);

  CrcLib::SetPwmOutput(CRC_PWM_1, powerA);
  CrcLib::SetPwmOutput(CRC_PWM_2, powerB);
}

//-------------------------- PRIVATES ----------------------------
/**
 * Set the shortest angle and spin direction in _moveParam
 * Also determines travel direction
 * @param currentAngle Wheel angle in radians
 * @param targetAngle Desired angle i radians
 */
void SwerveModule::getShortestAngle(double currentAngle, double targetAngle){
  //Reduce angles to only use half of the circle
  currentAngle = currentAngle > M_PI ? currentAngle - M_PI : currentAngle;
  targetAngle  = targetAngle  > M_PI ? targetAngle  - M_PI : targetAngle;

  //Calculate travel angle, will be negative
  _moveParam.shortest = targetAngle - currentAngle;
  _moveParam.dir = Direction::CLOCKWISE;

  //If the angle is to large take the shortest and reverse
  if(_moveParam.shortest < -M_PI_2){
    _moveParam.shortest = M_PI + _moveParam.shortest;
    _moveParam.dir = Direction::COUNTERCLOCKWISE;
  }
}

/**
 * Get the current wheel angle from the absolute encoder
 * @return angle in rads
 */
double SwerveModule::getCurrentAngle(){
  //Get encoder value, make sure the value is between 0 and 4160 (Observation de Félix: L'encodeur retour parfois 8000)
  double enco = constrain(pulseIn(CRC_PWM_12, HIGH), 0.0, 4160.0);

  //Calculate angle
  double angleAct = enco/4160.0 * (2*M_PI);
  return angleAct;
}
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
Vec2D SwerveModule::calculatePIDRad(double targetAngle, double tPower){
  //On s'assure que les valeurs passés en param sont conformes au range
  tPower = constrain(tPower, -1.0, 1.0);
  targetAngle = constrain(targetAngle, 0, 2*M_PI) / (2*M_PI); //On transforme en tour
  
  //Set shortest and dir in _moveParam
  getShortestAngleRad(getCurrentAngleRad(), targetAngle);


  //Set the translation power
  _vecPower.set_x(tPower);
    //TODO Scale translation power depending on angular power???


  return _vecPower;
}

/**
 * Calculate the power vector (translation, angular) of a Swerve module
 * @param targetAngle Desired angle of wheel in rads (0 to 2pi)
 * @param tPower Multiplication factor for translation power between -1.0 and 1.0
 * @return vecPower - {x=translationComponent, y=angularComponent}
 */
Vec2D SwerveModule::calculateRad(double targetAngle, double tPower){
  //On s'assure que les valeurs passés en param sont conformes au range
  // tPower = constrain(tPower, -1.0, 1.0);
  // targetAngle = constrain(targetAngle, 0, 2*M_PI) / (2*M_PI); //On transforme en tour
  
  //Set shortest and dir in _moveParam
  //getShortestAngleRad(getCurrentAngleRad(), targetAngle);
  getShortestAngleRad(0, targetAngle);

  //Find the absolute components of the power vector of the entire module
  _vecPower.set_y(getAngularComponentRad(fabs(_moveParam.shortest), fabs(tPower)));
  _vecPower.set_x(getTranslationComponentRad(fabs(_moveParam.shortest), fabs(tPower)));

  Serial.println("X" + String(_vecPower.x()));
  Serial.println("Y" + String(_vecPower.y()));

  //Apply the good sign
  switch(_moveParam.dir){
    case SwerveModule::Direction::CLOCKWISE:
      _vecPower.set_y(fabs(_vecPower.y()));
      break;
    case SwerveModule::Direction::COUNTERCLOCKWISE:
      _vecPower.set_y(-fabs(_vecPower.y()));
      break;
  }

  if(targetAngle > M_PI){
    //Backward
    _vecPower.set_x(-fabs(_vecPower.x()));
  }else{
    //Forward
    _vecPower.set_x(fabs(_vecPower.x()));
  }

  // switch(_moveParam.wheel){
  //   case SwerveModule::Direction::FORWARD:
  //     //Pas besoins d'inverser
  //     break;
  //   case SwerveModule::Direction::BACKWARD:
  //     _vecPower.set_x(-_vecPower.x());
  //     break;
  // }
  //Set the translation power
  //_vecPower.set_x(tPower);
    //TODO Scale translation power depending on angular power???


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
 * Set the shortest angle and spin direction in _moveParam
 * Also determines travel direction
 * @param currentAngle Wheel angle in radians
 * @param targetAngle Desired angle i radians
 */
void SwerveModule::getShortestAngleRad(double currentAngle, double targetAngle){
  //Reduce angles to only use half of the circle
  // currentAngle = currentAngle > M_PI ? currentAngle - M_PI : currentAngle;
  // targetAngle  = targetAngle  > M_PI ? targetAngle  - M_PI : targetAngle;

  //Calculate travel angle, will be negative
  _moveParam.shortest = targetAngle - currentAngle;
  _moveParam.dir = _moveParam.shortest < M_PI ? 
                                              _moveParam.shortest < M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE 
                                              : _moveParam.shortest < 3*M_PI/2 ? Direction::CLOCKWISE:Direction::COUNTERCLOCKWISE;
  
  //Keep value in a cercle
  while(_moveParam.shortest > 2*M_PI){
    _moveParam.shortest = _moveParam.shortest - 2*M_PI;
  }
  while(_moveParam.shortest < -2*M_PI){
    _moveParam.shortest = _moveParam.shortest + 2*M_PI;
  }

  //If the angle is to large take the shortest and reverse
  // if(_moveParam.shortest < -M_PI_2){
  //   _moveParam.shortest = M_PI + _moveParam.shortest;
  //   _moveParam.dir = Direction::COUNTERCLOCKWISE;
  // }
}

/**
 * Get the current wheel angle from the absolute encoder
 * @return angle in rads
 */
double SwerveModule::getCurrentAngleRad(){
  //Get encoder value, make sure the value is between 0 and 4160 (Observation de Félix: L'encodeur retour parfois 8000)
  double enco = constrain(pulseIn(CRC_PWM_12, HIGH), 0.0, 4160.0);

  //Calculate angle
  double angleAct = enco/4160.0 * (2*M_PI);
  return angleAct;
}

/**
 * Get the angular component using a ratio of the difference
 * between current and target angle, round to 0,001
 */
double SwerveModule::getAngularComponentRad(double shortest, double speedFactor){
    double component = sin(shortest)*speedFactor;

    //component = shortest > M_PI ? -component:component;
    //Serial.println("AngComp: " + String(component));
    return component;
}

/**
 * Get the translation component using a ratio of the difference
 * between current and target angle, round to 0,001
 */
double SwerveModule::getTranslationComponentRad(double shortest, double speedFactor){
    double component = cos(shortest)*speedFactor;
    //Serial.println("TrsComp: " + String(component));
    return component;
}

















//---------- FONCTIONS NON UTILISÉES: POTENTIELLEMENT INCORRECTES ------------ 

/**
 * Calculate the power vector (translation, angular) of a Swerve module
 * @param targetAngle Desired angle of wheel in rads (0 to 2pi)
 * @param tPower Multiplication factor for translation power between -1.0 and 1.0
 * @return vecPower - {x=translationComponent, y=angularComponent}
 */
Vec2D SwerveModule::calculateTurns(double targetAngle, double tPower){
  //On s'assure que les valeurs passés en param sont conformes au range
  tPower = constrain(tPower, -1.0, 1.0);
  targetAngle = constrain(targetAngle, 0, 2*M_PI) / (2*M_PI); //On transforme en tour

  _moveParam = getShortestAngleTurn(getCurrentAngleTurn(), targetAngle);
  //Serial.println(getCurrentAngle());

  //Find the absolute components of the power vector of the entire module
  _vecPower.set_y(getAngularComponent(fabs(_moveParam.shortest), fabs(tPower)));
  _vecPower.set_x(getTranslationComponent(fabs(_moveParam.shortest), fabs(tPower)));

  //Apply the good sign
  switch(_moveParam.dir){
    case SwerveModule::Direction::CLOCKWISE:
      //Pas besoins d'inverser
      break;
    case SwerveModule::Direction::COUNTERCLOCKWISE:
      _vecPower.set_y(-_vecPower.y());
      break;
  }

  switch(_moveParam.wheel){
    case SwerveModule::Direction::FORWARD:
      //Pas besoins d'inverser
      break;
    case SwerveModule::Direction::BACKWARD:
      _vecPower.set_x(-_vecPower.x());
      break;
  }

  return _vecPower;
}

//######################
//# Functions en tours #
//######################
/**
 * Set the shortest angle and spin direction in _moveParam
 * Also determines travel direction
 * @param currentAngle Wheel angle in turns
 * @param targetAngle Desired angle in turns
 */
SwerveModule::TravelParam SwerveModule::getShortestAngleTurn(double currentAngle, double targetAngle){
  auto shortest = targetAngle - currentAngle; // zero the target angle on the current angle
  if (shortest > 0.5)
  {
    shortest = shortest - 1;
  }
  else if (shortest < -0.5)
  {
    shortest = shortest + 1;
  }

  auto canOraOptimize = abs(shortest) > 0.25;
  if (canOraOptimize)
  {
    // antipodal optimization is the better name
    shortest += shortest > 0 ? -0.5 : +0.5;
  }
  
  return {
    shortest,
    canOraOptimize? Direction::FORWARD: Direction::BACKWARD,
    shortest>0? Direction::CLOCKWISE: Direction::COUNTERCLOCKWISE, 
  };
}

/**
 * Get the current wheel angle from the absolute encoder
 * @return angle in turns
 */
double SwerveModule::getCurrentAngleTurn(){
  const double PULSE_MAX = 4160; // in micros
  const double PULSE_MIN = 1;    // in micros
  auto pulse = pulseIn(CRC_PWM_12, HIGH);
  auto normalized = constrain(pulse - PULSE_MIN, 0.0, PULSE_MAX);
  auto ratio = normalized / PULSE_MAX;
  // we do 1 - ratio because the encoder reverses clockwise and counter clockwise
  return 1 - ratio;
}

/**
 * Get the angular component using a ratio of the difference
 * between current and target angle, round to 0,001
 */
double SwerveModule::getAngularComponent(double shortest, double speedFactor){
    double component = sin(shortest*2*M_PI)*speedFactor;
    //Serial.println("AngComp: " + String(component));
    return component;
}

/**
 * Get the translation component using a ratio of the difference
 * between current and target angle, round to 0,001
 */
double SwerveModule::getTranslationComponent(double shortest, double speedFactor){
    double component = cos(shortest*2*M_PI)*speedFactor;
    //Serial.println("TrsComp: " + String(component));
    return component;
}
#include <CrcLib.h>
#include "speedCalculation.hpp"
#include "math.h"
#include "math/vectors.hpp"
#include <PID_RT.h>
using math::cartesian::Vec2D;

PID_RT pid;
double rPower;
Vec2D vecPower;

/**
 * Initialise the default values for the calculations
 * Must be call in Setup()
 */
void SC::init(){
  pid.setPoint(0);
  pid.setReverse(false);
  pid.setOutputRange(-1,1);
  pid.setK(0.005, 0.00, 0.0); //Proportionnal, Integral, Derivative
  pid.start();
}

/**
 * Calculate the speed vector (translation,angular) of a Swerve module
 * @param targetAngle : desired angle of wheel between 0 and 360
 * @param tPower : Multiplication factor for speed between -1 and 1 for trnaslation
 * @return vecPower {angularComponent, translationComponent}
 */
Vec2D SC::calculate(double targetAngle, double speedFactor){
    Vec2D vecSpeed;

    double diffAngle = getDiffAngle(getCurrentAngle(), targetAngle);


    //Find the absolute components of the speed vector
    vecSpeed.set_y(getAngularComponent(fabs(diffAngle), fabs(speedFactor)));
    vecSpeed.set_x(getTranslationComponent(fabs(diffAngle), fabs(speedFactor)));

    //Apply the good sign
    vecSpeed.set_y(diffAngle < 0   ? vecSpeed.y()  : -vecSpeed.y());
    vecSpeed.set_x(speedFactor < 0 ? -vecSpeed.x() : vecSpeed.x());

    return vecPower;
}


/**
 * Converts degres to radians
 */
double degToRad(double deg){
    return deg * M_PI / 180;
}

/**
 * Get the difference between currentAngle and targetAngle
 * Sign indicate the rotation direction
 * @return Return an angle value between -90 and 90 deg
 */
double SC::getDiffAngle(double currentAngle, double targetAngle){
  auto travelAngle = targetAngle - currentAngle;

  //Change for shortest if to big
  if(travelAngle < -90){
    travelAngle = 180 + travelAngle;
  }

  return travelAngle;
}

/**
 * Get the angular component using a ratio of the difference
 * between current and target angle, round to 0,001
 */
double SC::getAngularComponent(double diffAngle, double speedFactor){
    double component = sin(degToRad(diffAngle))*speedFactor;
    Serial.println("AngComp: " + String(component));
    return component;
}

/**
 * Get the translation component using a ratio of the difference
 * between current and target angle, round to 0,001
 */
double SC::getTranslationComponent(double diffAngle, double speedFactor){
    double component = cos(degToRad(diffAngle))*speedFactor;
    Serial.println("TrsComp: " + String(component));
    return component;
}

/**
 * Returns the current wheel angle between 0 and 180 deg
 */
double SC::getCurrentAngle(){
  //Get encoder value, make sure the value is between 0 and 4160 (Observation de Félix: L'encodeur retour des fois 8000)
  double enco = constrain(pulseIn(CRC_PWM_12, HIGH), 0.0, 4160.0);

  //Calculate angle
  double angleAct = enco/4160.0*360;

  return angleAct;
}
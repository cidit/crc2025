#include "math.h"
#include "math/vectors.hpp"
#include "math/angles.hpp"
#include "util/looped.hpp"
#include "util/misc.hpp"
#include <PID_RT.h>
#include "drives/precision_motor.hpp"


//------- CONSTANTS -------


//---------------------- CONSTRUCTORS ---------------------------
/**
 * Must be called in Setup
 */
void PrecisionMotor::init(PID_RT pid, Mode mode, float ticks_per_rotation){
    _pid = pid;
    _mode = mode;
    _tpr = ticks_per_rotation;
}

//-------------------------- PUBLICS -----------------------------
/**
 * Does Nothing
 */
void PrecisionMotor::loop() {}

/**
 * Configure the internal value the motor must have
 * @param val Value in angle or RPM
 */
void PrecisionMotor::setValue(double val){
    if(_mode ==  Mode::ANGLE){
        _targetAngle = val;
    }
    else if(_mode == Mode::SPEED){
        _targetRPM = val;
    }
}

/**
 * Set the mode of the motor
 * @param mode PreciseMotor::Mode
 */
void PrecisionMotor::setMode(Mode mode){
    _mode = mode;
}

/**
 * Configure the parameters of the PID
 */
void PrecisionMotor::setTunings(PIDTunings tunings) {
    auto [p, i, d] = tunings;
    _pid.setK(p, i, d);
}


//-------------------------- Utils -----------------------------

math::Angle ticks_to_angle(float ticks, float ticks_per_rotation)
{
    auto wrapped = fmod(ticks, ticks_per_rotation);
    auto ratio = wrapped / ticks_per_rotation;
    return math::Angle::from_ratio(ratio);
}
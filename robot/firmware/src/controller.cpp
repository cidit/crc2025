#include <controller.hpp>
#include <CrcLib.h>
#include "math/vectors.hpp"
#include "math.h"
using math::cartesian::Vec2D;

/**
 * 
 */
Controller::Controller(){}


void Controller::update(){
    _vectorJoyLeft.set_x(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X));
    _vectorJoyLeft.set_y(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y));
    _angleJoyLeft = radToDeg(rads(_vectorJoyLeft));
    _normJoyLeft = cartToPolNorm(_vectorJoyLeft);

    _vectorJoyRight.set_x(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X));
    _vectorJoyRight.set_y(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y));
    _angleJoyRight = radToDeg(rads(_vectorJoyRight));
    _normJoyRight = cartToPolNorm(_vectorJoyRight);
}

//-------- Utility Functions ---------------
/**
 * Finds the angle of a vector in radians
 */
double Controller::rads(Vec2D vector) {
    int x = vector.x();
    int y = vector.y();

    double rads;
    rads = atan2(y, x);

    if (y == 1 && x == 0) {
        rads = 0;  // make other thing to repr no angle
        return rads;
    }
    if (x == 0 && y > 0) {
        rads = PI / 2;
        return rads;
    } else if (x == 0 && y < 1) {
        rads = 3 * PI / 2;
        return rads;
    } else if (x > 0 && y == 1) {
        rads = 0;
        return rads;
    } else if (x < 0 && y == 1) {
        rads = PI;
        return rads;
    }

    if (x < 0 && y < 1) {
        rads = atan2(y, x) + (2 * PI);
        return rads;
    } else if (x > 0 && y < 1) {
        rads = (atan2(y, x)) + 2 * PI;
        return rads;
    } else {
        return rads;
    }
}

/**
 * Convert Degrees to Radian
 */
double Controller::radToDeg(double radian){
    return radian*180/PI;
}

/**
 * Convert the norm of vector from cartesian to polar
 */
double Controller::cartToPolNorm(Vec2D vector)
{
    return sqrt(sq(vector.x()) + sq(vector.y()));
}
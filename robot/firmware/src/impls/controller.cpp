#include <controller.hpp>
#include <CrcLib.h>
#include "math/vectors.hpp"
#include "math.h"
using math::cartesian::Vec2D;

/**
 * Constructeur
 */
Controller::Controller(){}

/**
 * Update internal variables
 * Must called in loop
 */
void Controller::update(){
    calculateLeftJoy();
    read_buttons();
}

//-------- Getter Functions ----------------
/**
 * @return A struct containing information on the joystick
 */
void Controller::calculateLeftJoy(){
    //Get joystick position entre -1.0 et 1.0, rounded tom 0.0
    _joyLeft.x = -roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X)/12.80)/10.0;
    _joyLeft.y = roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)/12.80)/10.0;

    //On enleve le dead zone au centre du joy
    if(_joyLeft.x < 0.15 && _joyLeft.x > -0.15){
        _joyLeft.x = 0;
    }
    if(_joyLeft.y < 0.15 && _joyLeft.y > -0.15){
        _joyLeft.y = 0;
    }

    //Calcul de l'angle, ne calcule pas si le joy est au centre
    if(_joyLeft.x != 0 || _joyLeft.y != 0){
        _joyLeft.angleRad = atan2(_joyLeft.y, _joyLeft.x) + M_PI;
        _joyLeft.angleDeg = _joyLeft.angleRad * 180.0 / M_PI;
    }

    //Calcul de la norm 
    _joyLeft.norm = constrain(sqrt(sq(_joyLeft.x)+sq(_joyLeft.y)), -1.0, 1.0);
}

//-------- Utility Functions ---------------
/**
 * Finds the angle of a vector in radians
 */
double Controller::rads(Joy j) {
    int x = j.x;
    int y = j.y;

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

/**
 * Read and store value of all buttons
 */
void Controller::read_buttons(){
    _X = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
    _T = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
}
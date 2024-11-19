#include <controller.hpp>
#include <CrcLib.h>
#include "math/vectors.hpp"
using math::cartesian::Vec2D;

/**
 * 
 */
Controller::Controller(){}

/**
 * 
 */
Vec2D Controller::get_translation_vec(){
    Vec2D vector;
    vector.set_x(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X));
    vector.set_y(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y));
    return vector;
}

/**
 * 
 */
Vec2D Controller::get_rotation_vec(){
    Vec2D vector;
    vector.set_x(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X));
    vector.set_y(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y));
    return vector;
}
#pragma once

#include "math/vectors.hpp"
using math::cartesian::Vec2D;

class Controller{

    public:
        Controller();
        void update();

        //Getter Functions
        Vec2D get_left_joy_vec() {return _vectorJoyLeft;}
        double get_left_joy_angle() {return _angleJoyLeft;}
        double get_left_joy_norm() {return _normJoyLeft;}

        Vec2D get_right_joy_vec() {return _vectorJoyRight;}
        double get_right_joy_angle() {return _angleJoyRight;}
        double get_right_joy_norm() {return _normJoyRight;}
    
    private:
        //Functions
        double rads(Vec2D vector);
        double radToDeg(double radian);
        double cartToPolNorm(Vec2D vector);

        // Joystick Gauche
        Vec2D _vectorJoyLeft;
        double _angleJoyLeft;
        double _normJoyLeft;

        // Joystick Droit
        Vec2D _vectorJoyRight;
        double _angleJoyRight;
        double _normJoyRight;
};
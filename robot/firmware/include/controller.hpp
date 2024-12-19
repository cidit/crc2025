#pragma once

#include "math/vectors.hpp"
using math::cartesian::Vec2D;

class Controller{

    public:
        struct Joy{
            double x;
            double y;
            double norm;
            double angleDeg;
            double angleRad;
        };
        Controller();
        void update();

        //Getter Functions
        Joy get_left_joy() {return _joyLeft;}
        Joy get_right_joy_vec() {return _joyRight;}
        bool get_X() {return _X;}
        bool get_T() {return _T;}

    private:
        //Functions
        double rads(Joy j);
        double radToDeg(double radian);
        double cartToPolNorm(Vec2D vector);
        void calculateLeftJoy();
        void calculateRightJoy();
        void read_buttons();

        // Values for Joystick
        Joy _joyLeft;
        Joy _joyRight;
        
        //Buttons vars
        bool _X = false;
        bool _T = false;
};
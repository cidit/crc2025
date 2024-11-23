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

    private:
        //Functions
        //double rads(Joy j);
        double radToDeg(double radian);
        double cartToPolNorm(Vec2D vector);
        void calculateLeftJoy();
        void calculateRightJoy();
        float mapf(float x, float in_min, float in_max, float out_min, float out_max);

        // Values for Joystick
        Joy _joyLeft;
        Joy _joyRight;
};
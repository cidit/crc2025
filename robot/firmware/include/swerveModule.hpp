#pragma once

#include "math.h"
#include "math/vectors.hpp"
#include <PID_RT.h>
using math::cartesian::Vec2D;

class SwerveModule
{
    public:
        enum class Direction
        {
            CLOCKWISE,
            COUNTERCLOCKWISE,
            FORWARD,
            BACKWARD,
        };

        struct TravelParam
        {
            double shortest;
            Direction wheel; // if the WHEEL of the swerve reversed FWD/BWD
            Direction dir; // the direction of the rotation
        };

        void init(double Kp, double Ki, double Kd);
        bool calculateAndApply(double targetAngle, double tPower);
        Vec2D calculateRad(double targetAngle, double tPower);
        void setMotorPowers(Vec2D powerVector);
    
    private:
        PID_RT _pid;
        Vec2D _vecPower; //x=trans, y=angular
        TravelParam _moveParam;

        void getShortestAngleRad(double currentAngle, double targetAngle);
        double getCurrentAngleRad();

        double getAngularComponentRad(double shortest, double speedFactor);
        double getTranslationComponentRad(double shortest, double speedFactor);
};

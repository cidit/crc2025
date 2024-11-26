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
            float shortest;
            Direction wheel; // if the WHEEL of the swerve reversed FWD/BWD
            Direction dir; // the direction of the rotation
        };

        void init(double Kp, double Ki, double Kd);
        bool calculateAndApply(double targetAngle, double tPower);
        Vec2D calculatePIDRad(double targetAngle, double speedFactor);
        Vec2D calculateRad(double targetAngle, double tPower);
        Vec2D calculateTurns(double targetAngle, double speedFactor);
        void setMotorPowers(Vec2D powerVector);
    
    private:
        PID_RT _pid;
        Vec2D _vecPower; //x=trans, y=angular
        TravelParam _moveParam;

        TravelParam getShortestAngleTurn(double currentAngle, double targetAngle);
        double getCurrentAngleTurn();

        void getShortestAngleRad(double currentAngle, double targetAngle);
        double getCurrentAngleRad();

        double getAngularComponent(double shortest, double speedFactor);
        double getTranslationComponent(double shortest, double speedFactor);

        double getAngularComponentRad(double shortest, double speedFactor);
        double getTranslationComponentRad(double shortest, double speedFactor);
};

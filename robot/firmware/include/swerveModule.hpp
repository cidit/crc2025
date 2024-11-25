#pragma once


class SwerveModule
{
    public:
        enum class Direction
        {
            CLOCKWISE,
            COUNTERCLOCKWISE
        };

        struct TravelParam
        {
            float shortest;
            Direction dir;
        };

        void init(double Kp, double Ki, double Kd);
        bool calculateAndApply(double targetAngle, double tPower);
        Vec2D calculate(double targetAngle, double speedFactor);
        void setMotorPowers(Vec2D powerVector);
    
    private:
        PID_RT _pid;
        Vec2D _vecPower; //x=trans, y=angular
        TravelParam _moveParam;

        void getShortestAngle(double currentAngle, double targetAngle);
        double getCurrentAngle();
};
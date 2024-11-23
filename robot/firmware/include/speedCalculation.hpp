#pragma once

#include "math/vectors.hpp"

using math::cartesian::Vec2D;

namespace SC
{
    struct speedVector{
        double angularComponent;
        double translationComponent;
    };

    void init();
    Vec2D calculate(double targetAngle, double speedFactor);
    double getDiffAngle(double targetAngle, double currentAngle);
    double getAngularComponent(double diffAngle, double speedFactor);
    double getTranslationComponent(double diffAngle, double speedFactor);
    double getCurrentAngle();
}
#pragma once
#include "drives/precision_motor2.hpp"
#include "math/vectors.hpp"

class SwerveModule : public Lifecycle
{
private:
    PrecisionMotor &_pma, &_pmb;

public:
    SwerveModule(PrecisionMotor &pma, PrecisionMotor &pmb)
        : _pma(pma),
          _pmb(pmb)
    {
    }

    void begin() override
    {
        // TODO: unimplemented
    }

    void update() override
    {
        // TODO: unimplemented
    }

    /**
     * @param target a vec that encodes a direction as an angle and a speed in RPMs
     */
    void set_target(math::cartesian::Vec2D target)
    {
        // TODO: unimplemented
    }

    double get_angle()
    {
        return -1; // TODO: unimplemented
    }
};
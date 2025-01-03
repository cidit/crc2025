#pragma once
#include "drives/precision_motor2.hpp"
#include "math/vectors.hpp"

class SwerveModule : public Lifecycle
{
private:
    PrecisionMotor &_pma, &_pmb;
    pin_t _e_p;

public:
    SwerveModule(PrecisionMotor &pma, PrecisionMotor &pmb, pin_t abs_enco_pin)
        : _pma(pma),
          _pmb(pmb),
          _e_p(abs_enco_pin)
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
        // TODO: handle the encoder externally so that the pulse length can be ajusted if need be.
        const auto MAX_PULSE_LEN = 4160.0;
        double pulse = pulseIn(_e_p, HIGH, MAX_PULSE_LEN*2); // TODO: if there are issues later, check the pulseIn
        double angle = (pulse / MAX_PULSE_LEN) * (2 * M_PI);
        return angle;
    }
};
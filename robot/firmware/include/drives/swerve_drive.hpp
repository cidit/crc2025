#pragma once
#include "util/lifecycle.hpp"
#include "drives/swerve_module.hpp"

class SwerveDrive : public Lifecycle
{
public:
    SwerveModule &_r, &_l;

    swerve_heading _target;

    bool _enabled;

    SwerveDrive(SwerveModule &right, SwerveModule &left)
        : _r(right), _l(left), _target{.direction = 0, .velocity = 0}
    {
        // no-op
    }

    void begin() override {
        // TODO: unimplemented
    }

    void set_target(swerve_heading target) {
        // TODO: unimplemented
    }

    void update() override {
        // TODO: unimplemented
    }

    void enable(bool enabled) {
        // TODO: unimplemented
    }

};
#pragma once
#include "util/lifecycle.hpp"
#include "drives/swerve_module.hpp"
#include "util/constants.hpp"

struct swerve_drive_heading
{
    swerve_heading swerve_heading;
    double rotation; // in rpms
};

class SwerveDrive : public Lifecycle
{
public:
    SwerveModule &_r, &_l;

    swerve_drive_heading _target;

    bool _enabled;

    SwerveDrive(SwerveModule &right, SwerveModule &left)
        : _r(right),
          _l(left),
          _target{.swerve_heading = {.direction = 0, .velocity = 0}, .rotation = 0},
          _enabled(false)
    {
        // no-op
    }

    SwerveDrive(const SwerveDrive &) = delete;
    SwerveDrive &operator=(const SwerveDrive &) = delete;

    void begin() override
    {
        _r.begin();
        _l.begin();
    }

    void set_target(swerve_drive_heading target)
    {
        _target = target;
    }

    void update() override
    {
        // determine the cartesian values of the target
        auto target_as_vect = Vec2D::from_polar(
            _target.swerve_heading.direction,
            _target.swerve_heading.velocity);

        // determine the strength of the rotation 
        auto right_rot_as_vec = Vec2D(0, -_target.rotation);
        auto left_rot_as_vec = Vec2D(0, _target.rotation);

        // average the target and the rotation vectors to find the resulting norm
        auto right_resultant_norm = Vec2D((target_as_vect.x() + right_rot_as_vec.x()) / 2,
                                          (target_as_vect.y() + right_rot_as_vec.y()) / 2)
                                        .norm();
        auto left_resultant_norm = Vec2D((target_as_vect.x() + left_rot_as_vec.x()) / 2,
                                         (target_as_vect.y() + left_rot_as_vec.y()) / 2)
                                       .norm();
        
        // average the target and rotations to find the resulting angle
        auto right_direction = (_target.swerve_heading.direction + right_rot_as_vec.angle()) / 2;
        auto left_direction = (_target.swerve_heading.direction + left_rot_as_vec.angle()) / 2;        

        // rescale the norms if they exceed their maximum rpms (which would cause the other to lag behind)
        if (_r.get_max_linear_velocity() < right_resultant_norm) {
            auto ratio = _r.get_max_linear_velocity()/right_resultant_norm;
            right_resultant_norm = _r.get_max_linear_velocity();
            left_resultant_norm *= ratio;
        }
        if (_l.get_max_linear_velocity() < left_resultant_norm) {
            auto ratio = _l.get_max_linear_velocity()/left_resultant_norm;
            left_resultant_norm = _l.get_max_linear_velocity();
            right_resultant_norm *= ratio;
        }

        // update swerve module targets
        _r.set_target({
            .direction = right_direction,
            .velocity = right_resultant_norm
        });
        _l.set_target({
            .direction = left_direction,
            .velocity = left_resultant_norm
        });

        // update swerve modules
        _r.update();
        _l.update();
    }

    void enable(bool enabled)
    {
        _enabled = enabled;
        _r.enable(enabled);
        _l.enable(enabled);
    }
};
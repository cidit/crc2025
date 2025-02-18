#pragma once
#include "util/lifecycle.hpp"
#include "util/display_name.hpp"
#include "drives/swerve_module.hpp"
#include "util/constants.hpp"

struct swerve_drive_heading
{
    swerve_heading heading;
    double rotation; // in rpms
};

class SwerveDrive : public Lifecycle
{
public:
    // the "left" module is forwards facing, whereas the right module is backwards facing. important in calculations
    SwerveModule &_r, &_l;

    swerve_drive_heading _target;

    bool _enabled;

    SwerveDrive(SwerveModule &right, SwerveModule &left)
        : _r(right),
          _l(left),
          _target{.heading = {.direction = 0, .velocity = 0}, .rotation = 0},
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
            _target.heading.direction,
            _target.heading.velocity);


        // the reference frames for each swerve is rotated by 90* compared to the robot's
        auto target_left_module = target_as_vect.rotate_by(-M_PI_2);
        auto target_right_module = target_as_vect.rotate_by(M_PI_2);


        // determine the strength of the rotation. (if rotation is positive, rotate clockwise)
        auto rot_as_vec = Vec2D(_target.rotation, 0);

        // average the target and the rotation vectors to find the resulting norm
        auto left_resultant = Vec2D((target_left_module.x() + rot_as_vec.x()) / 2,
                                         (target_left_module.y() + rot_as_vec.y()) / 2);
        auto right_resultant = Vec2D((target_right_module.x() + rot_as_vec.x()) / 2,
                                          (target_right_module.y() + rot_as_vec.y()) / 2);


        auto left_norm = left_resultant.norm();
        auto right_norm = right_resultant.norm();

        // average the target and rotations to find the resulting angle
        // auto left_direction = (_target.heading.direction + rot_as_vec.angle()) / 2;
        // auto right_direction = (Angle::from_rad(_target.heading.direction - M_PI)._radians + rot_as_vec.angle()) / 2;


        // rescale the norms if they exceed their maximum rpms (which would cause the other to lag behind)
        if (_l.get_max_linear_velocity() < left_norm)
        {
            auto ratio = _l.get_max_linear_velocity() / left_norm;
            left_norm = _l.get_max_linear_velocity();
            left_norm *= ratio;
        }
        if (_r.get_max_linear_velocity() < right_norm)
        {
            auto ratio = _r.get_max_linear_velocity() / right_norm;
            right_norm = _r.get_max_linear_velocity();
            right_norm *= ratio;
        }

        // update swerve module targets
        _l.set_target({.direction = left_resultant.angle(),
                       .velocity = left_norm});
        _r.set_target({.direction = right_resultant.angle(),
                       .velocity = right_norm});

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
#pragma once
#include "math/vectors.hpp"
#include "util/lifecycle.hpp"
#include "CrcRemoteState.h"
#include "CrcLib.h"
#include "CrcXbee.h"
#include <Bouton.h>
#include "util/constants.hpp"

/**
 * largely based off of @squid's controller
 */
class Controller : public Lifecycle
{
public:

    /**
     * this struct's angle field gets updated if it's
     * raw values' norm is 0.
     */
    struct Joystick
    {
        Vec2D xy;
        double angle; // in rads
    };

    CrcUtility::RemoteState _raw;

    Bouton arrow_up, arrow_down, arrow_left, arrow_right,
        color_up, color_down, color_left, color_right,
        hat_left, hat_right,
        logo, select, start,
        left_bumper, right_bumper;   
    double left_trigger, right_trigger;
    Joystick joystick_left, joystick_right;

    /// @brief Constructor
    Controller() {}
    Controller(const Controller &) = delete;
    Controller &operator=(const Controller &) = delete;

    void begin() override
    {
        arrow_up = Bouton(_raw.arrowUp);
        arrow_down = Bouton(_raw.arrowDown);
        arrow_right = Bouton(_raw.arrowRight);
        arrow_left = Bouton(_raw.arrowLeft);
        color_up = Bouton(_raw.colorUp);
        color_down = Bouton(_raw.colorDown);
        color_right = Bouton(_raw.colorRight);
        color_left = Bouton(_raw.colorLeft);
        hat_left = Bouton(_raw.hatL);
        hat_right = Bouton(_raw.hatR);
        logo = Bouton(_raw.logo);
        select = Bouton(_raw.select);
        start = Bouton(_raw.start);
        left_bumper = Bouton(_raw.L1);
        right_bumper = Bouton(_raw.R1);
        left_trigger = 0;
        right_trigger = 0;
        joystick_left = {.xy = {0, 0}, .angle = 0};
        joystick_right = {.xy = {0, 0}, .angle = 0};
    }

    void update() override
    {
        _raw = CrcLib::_crcXbee.State();
        _patch_raw();
        arrow_up.refresh();
        arrow_down.refresh();
        arrow_right.refresh();
        arrow_left.refresh();
        color_up.refresh();
        color_down.refresh();
        color_right.refresh();
        color_left.refresh();
        hat_left.refresh();
        hat_right.refresh();
        logo.refresh();
        select.refresh();
        left_bumper.refresh();
        right_bumper.refresh();

        left_trigger = double(_raw.gachetteG + HALF_PWM_OUTPUT) / HALF_PWM_OUTPUT;
        right_trigger = double(_raw.gachetteD + HALF_PWM_OUTPUT) / HALF_PWM_OUTPUT;

        joystick_left = _update_joystick(_raw.joystick1X, _raw.joystick1Y, joystick_left);
        joystick_right = _update_joystick(_raw.joystick2X, _raw.joystick2Y, joystick_right);
    }

    /// apply corrections to the inputs
    void _patch_raw() {
        // fixed analog channels the same way crclib does it under the hood
        _raw.joystick1X -= 128;
        _raw.joystick1Y -= 128;
        _raw.joystick2X -= 128;
        _raw.joystick2Y -= 128;
        _raw.gachetteD -= 128;
        _raw.gachetteG -= 128;

        //  /!\   y values of joysticks are inverted for some reason
        _raw.joystick1Y = -_raw.joystick1Y;
        _raw.joystick2Y = -_raw.joystick2Y;
    }

    Joystick _update_joystick(double x, double y, const Joystick &old_state)
    {
        auto xy_vec = Vec2D(x / HALF_PWM_OUTPUT,
                            y / HALF_PWM_OUTPUT);
        auto norm = xy_vec.norm();
        constexpr auto CTLR_DEADZONE = 0.15;
        auto in_deadzone = abs(norm) < CTLR_DEADZONE;

        return in_deadzone
                   // on conserve le vieil angle et on met xy à 0
                   ? (Joystick){.xy = Vec2D(0, 0), .angle = joystick_left.angle}
                   : (Joystick){.xy = xy_vec, .angle = xy_vec.angle()};
    }
};

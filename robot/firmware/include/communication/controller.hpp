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
class Controller// : public Lifecycle
{
public:
    struct Joy
    {
        double x = 0;
        double y = 0;
        double norm = 0;
        double angleDeg = 0;
        double angleRad = 0;
    };

    struct Buttons
    {
        bool X = false;
        bool Y = false;
        bool A = false;
        bool B = false;
        bool Up = false;
        bool Down = false;
        bool Left = false;
        bool Right = false;
        bool LBumper = false;
        bool RBumper = false;
        bool LJoy = false;
        bool RJoy = true;
    };

    struct Gachettes
    {
        float Right;
        float Left;
    };

    // Joysticks
    Joy joyLeft;
    Joy joyRight;

    // Buttons
    Buttons buttons;

    // Gachettes
    Gachettes gachettes;

    // /**
    //  * this struct's angle field gets updated if it's
    //  * raw values' norm is 0.
    //  */
    // struct Joystick
    // {
    //     Vec2D xy;
    //     double angle; // in rads
    // };

    // CrcUtility::RemoteState _raw;

    // Bouton arrow_up, arrow_down, arrow_left, arrow_right,
    //     color_up, color_down, color_left, color_right,
    //     hat_left, hat_right,
    //     logo, select, start,
    //     left_1, right_1;    // boutons des gachettes
    // double left_2, right_2; // gachettes
    // Joystick joystick_left, joystick_right;

    // /// @brief Constructor
    // Controller() {}
    // Controller(const Controller &) = delete;
    // Controller &operator=(const Controller &) = delete;

    // void begin() override
    // {
    //     arrow_up = Bouton(_raw.arrowUp);
    //     arrow_down = Bouton(_raw.arrowDown);
    //     arrow_right = Bouton(_raw.arrowRight);
    //     arrow_left = Bouton(_raw.arrowLeft);
    //     color_up = Bouton(_raw.colorUp);
    //     color_down = Bouton(_raw.colorDown);
    //     color_right = Bouton(_raw.colorRight);
    //     color_left = Bouton(_raw.colorLeft);
    //     hat_left = Bouton(_raw.hatL);
    //     hat_right = Bouton(_raw.hatR);
    //     logo = Bouton(_raw.logo);
    //     select = Bouton(_raw.select);
    //     start = Bouton(_raw.start);
    //     left_1 = Bouton(_raw.L1);
    //     right_1 = Bouton(_raw.R1);
    //     left_2 = 0;
    //     right_2 = 0;
    //     joystick_left = {.xy = {0, 0}, .angle = 0};
    //     joystick_right = {.xy = {0, 0}, .angle = 0};
    // }

    // void update() override
    // {
    //     _raw = CrcLib::_crcXbee.State();

    //     arrow_up.refresh();
    //     arrow_down.refresh();
    //     arrow_right.refresh();
    //     arrow_left.refresh();
    //     color_up.refresh();
    //     color_down.refresh();
    //     color_right.refresh();
    //     color_left.refresh();
    //     hat_left.refresh();
    //     hat_right.refresh();
    //     logo.refresh();
    //     select.refresh();
    //     left_1.refresh();
    //     right_1.refresh();

    //     left_2 = (_raw.gachetteG + HALF_PWM_OUTPUT) / HALF_PWM_OUTPUT;
    //     right_2 = (_raw.gachetteD + HALF_PWM_OUTPUT) / HALF_PWM_OUTPUT;

    //     joystick_left = _update_joystick(_raw.joystick1X, _raw.joystick1Y, joystick_left);
    //     joystick_right = _update_joystick(_raw.joystick2X, _raw.joystick2Y, joystick_right);
    // }

    // Joystick _update_joystick(int8_t x, int8_t y, const Joystick &old_state)
    // {
    //     auto xy_vec = Vec2D(x / HALF_PWM_OUTPUT,
    //                         y / HALF_PWM_OUTPUT);
    //     auto norm = xy_vec.norm();
    //     constexpr auto CTLR_DEADZONE = 0.15;
    //     auto in_deadzone = abs(xy_vec.norm()) < CTLR_DEADZONE;

    //     return in_deadzone
    //                // on conserve le vieil angle et on met xy à 0
    //                ? (Joystick){.xy = Vec2D(0, 0), .angle = joystick_left.angle}
    //                : (Joystick){.xy = xy_vec, .angle = xy_vec.angle()};
    // }

    void update()
    {
        calculateJoys();
        readButtons();
    }

    /// @brief Get and calculate values for the joysticks
    void calculateJoys()
    {
        // Get joystick position entre -1.0 et 1.0, rounded tom 0.0
        joyLeft.x = -roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X) / 12.80) / 10.0;
        joyLeft.y = roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y) / 12.80) / 10.0;
        joyRight.x = -roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X) / 12.80) / 10.0;
        joyRight.y = roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y) / 12.80) / 10.0;

        // On enleve le dead zone au centre du joy
        if (joyLeft.x < 0.15 && joyLeft.x > -0.15)
        {
            joyLeft.x = 0;
        }
        if (joyLeft.y < 0.15 && joyLeft.y > -0.15)
        {
            joyLeft.y = 0;
        }
        if (joyRight.x < 0.15 && joyRight.x > -0.15)
        {
            joyRight.x = 0;
        }
        if (joyRight.y < 0.15 && joyRight.y > -0.15)
        {
            joyRight.y = 0;
        }

        // Calcul de l'angle, ne calcule pas si le joy est au centre
        if (joyLeft.x != 0 || joyLeft.y != 0)
        {
            joyLeft.angleRad = atan2(joyLeft.y, joyLeft.x) + M_PI;
            joyLeft.angleDeg = joyLeft.angleRad * 180.0 / M_PI;
        }
        if (joyRight.x != 0 || joyRight.y != 0)
        {
            joyRight.angleRad = atan2(joyRight.y, joyRight.x) + M_PI;
            joyRight.angleDeg = joyRight.angleRad * 180.0 / M_PI;
        }

        // Calcul de la norm
        joyLeft.norm = constrain(sqrt(sq(joyLeft.x) + sq(joyLeft.y)), -1.0, 1.0);
        joyRight.norm = constrain(sqrt(sq(joyRight.x) + sq(joyRight.y)), -1.0, 1.0);
    }

    /// @brief Get values of buttons
    void readButtons()
    {
        buttons.A = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
        buttons.Y = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
        buttons.X = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
        buttons.B = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
        buttons.Up = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
        buttons.Down = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
        buttons.Left = CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT);
        buttons.Right = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
        buttons.LJoy = CrcLib::ReadDigitalChannel(BUTTON::HATL);
        buttons.RJoy = CrcLib::ReadDigitalChannel(BUTTON::HATR);
        buttons.LBumper = CrcLib::ReadDigitalChannel(BUTTON::L1);
        buttons.RBumper = CrcLib::ReadDigitalChannel(BUTTON::R1);

        // Quand le bouton est relacher la valeur est -128 et une fois enfoncer la valeur est 127
        // Mapping entre 0 et 1
        gachettes.Left = (CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L) + 128.0) / 256.0;
        gachettes.Right = (CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R) + 128.0) / 256.0;
    }
};

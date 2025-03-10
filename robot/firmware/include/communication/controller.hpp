#pragma once
#include "math/vectors.hpp"
#include "util/lifecycle.hpp"
#include "CrcRemoteState.h"
#include "CrcLib.h"
#include "CrcXbee.h"
#include <Bouton.h>
#include "util/constants.hpp"

class Controller : public Lifecycle
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

    /// @brief Constructor
    Controller() {}
    Controller(const Controller &) = delete;
    Controller &operator=(const Controller &) = delete;

    void begin() override
    {
        // no-op
    }

    void update() override
    {
        calculateJoys();
        readButtons();
    }

    // @brief Get and calculate values for the joysticks
    void calculateJoys()
    {
        // Get joystick position entre -1.0 et 1.0, rounded tom 0.0
        joyLeft.x = -roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X) / 12.80) / 10.0;
        joyLeft.y = roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y) / 12.80) / 10.0;
        joyRight.x = -roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X) / 12.80) / 10.0;
        joyRight.y = roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y) / 12.80) / 10.0;

        // Calcul de la norm
        joyLeft.norm = constrain(sqrt(sq(joyLeft.x) + sq(joyLeft.y)), -1.0, 1.0);
        joyRight.norm = constrain(sqrt(sq(joyRight.x) + sq(joyRight.y)), -1.0, 1.0);
        
        if (!CrcLib::IsCommValid()) {
            joyLeft.x = joyLeft.y = joyRight.x = joyRight.y = 0;
            return;
        }

        // On enleve le dead zone au centre du joy
        constexpr auto DEADZONE = 0.65;
        if (abs(joyLeft.norm) < DEADZONE) {
            joyLeft.x = joyLeft.y = joyLeft.norm = 0;
        }
        if (abs(joyRight.norm) < DEADZONE) {
            joyRight.x = joyRight.y = joyRight.norm = 0;
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

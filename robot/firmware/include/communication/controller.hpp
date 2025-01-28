#pragma once
#include "math/vectors.hpp"
#include "util/lifecycle.hpp"
#include "CrcRemoteState.h"
#include "CrcLib.h"
#include "CrcXbee.h"

/**
 * largely based off of @squid's controller
 */
class Controller: public Lifecycle
{
public:
    struct Joy{
        double x = 0;
        double y = 0;
        double norm = 0;
        double angleDeg = 0;
        double angleRad = 0;
    };

    struct Buttons{
        bool X = false;
        bool T = false;
        bool C = false;
        bool R = false;
        bool Up = false;
        bool Down = false;
        bool Left = false;
        bool Right = false;
    };

    struct Gachettes{
        int i; //Placeholder
    };

    //Joysticks
    Joy joyLeft;
    Joy joyRight;

    //Buttons
    Buttons buttons;

    /// @brief Constructor
    Controller() {}
    
    void begin() override {
        //No-Op
    }

    void update() override {
        calculateJoys();
        readButtons();
    }

    /// @brief Get and calculate values for the joysticks
    void calculateJoys(){
        //Get joystick position entre -1.0 et 1.0, rounded tom 0.0
        joyLeft.x = -roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X)/12.80)/10.0;
        joyLeft.y = roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)/12.80)/10.0;
        joyRight.x = -roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X)/12.80)/10.0;
        joyRight.y = roundf(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y)/12.80)/10.0;

        //On enleve le dead zone au centre du joy
        if(joyLeft.x < 0.15 && joyLeft.x > -0.15){
            joyLeft.x = 0;
        }
        if(joyLeft.y < 0.15 && joyLeft.y > -0.15){
            joyLeft.y = 0;
        }
        if(joyRight.x < 0.15 && joyRight.x > -0.15){
            joyRight.x = 0;
        }
        if(joyRight.y < 0.15 && joyRight.y > -0.15){
            joyRight.y = 0;
        }

        //Calcul de l'angle, ne calcule pas si le joy est au centre
        if(joyLeft.x != 0 || joyLeft.y != 0){
            joyLeft.angleRad = atan2(joyLeft.y, joyLeft.x) + M_PI;
            joyLeft.angleDeg = joyLeft.angleRad * 180.0 / M_PI;
        }
        if(joyRight.x != 0 || joyRight.y != 0){
            joyRight.angleRad = atan2(joyRight.y, joyRight.x) + M_PI;
            joyRight.angleDeg = joyRight.angleRad * 180.0 / M_PI;
        }

        //Calcul de la norm 
        joyLeft.norm = constrain(sqrt(sq(joyLeft.x)+sq(joyLeft.y)), -1.0, 1.0);
        joyRight.norm = constrain(sqrt(sq(joyRight.x)+sq(joyRight.y)), -1.0, 1.0);
    }

    /// @brief Get values of buttons
    void readButtons(){
        buttons.X = CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN);
        buttons.T = CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP);
        buttons.C = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
        buttons.R = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
        buttons.Up = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
        buttons.Down = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
        buttons.Left = CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT);
        buttons.Right = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
    }
};
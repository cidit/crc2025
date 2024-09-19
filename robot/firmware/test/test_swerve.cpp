#include <XBee.h>
#include <CrcLib.h>
#include "CrcRemoteState.h"
#include <Encoder.h>
#include <CrcLib/CrcBuzz.h>
#include "drives/swerve.hpp"
#include <Decodeur.h>
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
#include "math/angles.hpp"

Decodeur decoder(&Serial);
int ticks_per_turn = 4700 / 5;

XboxSeriesXControllerESP32_asukiaaa::Core xboxController;

math::Angle x_y_to_angle(float x, float y)
{
    if (y == 1 && x == 0)
    {
        return math::Angle::zero();
    }
    if (x == 0 && y > 0)
    {
        return math::Angle::from_rad(PI / 2);
    }
    if (x == 0 && y < 1)
    {
        return math::Angle::from_rad(3 * PI / 2);
    }
    if (x > 0 && y == 1)
    {
        return math::Angle::zero();
    }
    if (x < 0 && y == 1)
    {
        return math::Angle::from_rad(PI);
    }
    if ((x != 0) && y < 1)
    {
        return math::Angle::from_rad(atan2(y, x) + (2 * PI));
    }
    return math::Angle::from_rad(atan2(y, x));
}

void setup()
{
    Serial.begin(115200); // Open the serial Monitor at a 2000000 baud rate
    Serial.println("Starting NimBLE Client");
    xboxController.begin();
}
void loop()
{
    xboxController.onLoop();
    if (xboxController.isConnected())
    {
        if (xboxController.isWaitingForFirstNotification())
        {
            Serial.println("waiting for first notification");
        }
        else
        {
            uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;
            Serial.print("joyLVert rate: ");
            Serial.print((float)xboxController.xboxNotif.joyLVert / joystickMax);
            Serial.print(String("   ||   ") +String( "joyLHori rate: "));
            Serial.print((float)xboxController.xboxNotif.joyLHori / joystickMax);
        }
    }
    else
    {
        Serial.println("not connected");
        if (xboxController.getCountFailedConnection() > 2)
        {
            ESP.restart();
        }
    }
}
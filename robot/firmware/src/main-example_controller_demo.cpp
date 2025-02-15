
#include <CrcLib.h>
#include <communication/experimental_controller.hpp>
#include "util/print_crc_extras.hpp"

Controller controller;

void setup()
{
    Serial.begin(115200);
    CrcLib::Initialize();
    Serial.println("Setup Done");
    controller.begin();
}

void loop()
{
    CrcLib::Update();
    controller.update();

    // controller._raw.PrintPayload();
    SEPARATOR;
    // print_battery();
    // SEPARATOR;
    // SPRINT(controller._raw.joystick1X);
    // SEPARATOR;
    // SPRINT(controller._raw.joystick1Y);
    // SEPARATOR;
    // SPRINT(controller._raw.joystick2X);
    // SEPARATOR;
    // SPRINT(controller._raw.joystick2Y);
    // SEPARATOR;

    // SPRINT(controller.joystick_left.xy.x());
    // SEPARATOR;
    // SPRINT(controller.joystick_left.xy.y());
    // SEPARATOR;
    // SPRINT(controller.joystick_right.xy.x());
    // SEPARATOR;
    // SPRINT(controller.joystick_right.xy.y());
    // SEPARATOR;

    SPRINT(" L:");
    SPRINT(" A:");
    SPRINT(controller.joystick_left.angle);
    SPRINT(" N:");
    SPRINT(controller.joystick_left.xy.norm());
    SPRINT(" T:");
    SPRINT(controller.left_trigger);
    SPRINT(" B:");
    SPRINT(controller.left_bumper.isPressed());
    SEPARATOR;


    SPRINT(" R:");
    SPRINT(" A:");
    SPRINT(controller.joystick_right.angle);
    SPRINT(" N:");
    SPRINT(controller.joystick_right.xy.norm());
    SPRINT(" T:");
    SPRINT(controller.right_trigger);
    SPRINT(" B:");
    SPRINT(controller.right_bumper.isPressed());
    SEPARATOR;

    SPRINT("\n");

    // SPRINT("      00%                 00%     ");
    // SPRINT("   _/ l1  \\_____________/  r1 \\_  ");
    // SPRINT(" /                               \\");
    // SPRINT("|      ^                   b      |");
    // SPRINT("|    < + >     s l s     a + d    |");
    // SPRINT("|      v                   c      |");
    // SPRINT("|                                 |");
    // SPRINT("|     \\ | /             \\ | /     |");
    // SPRINT("|     - o -     ___     - o -     |");
    // SPRINT("|     / | \\   /     \\   / | \\     |");
    // SPRINT("|            /       \\            |");
    // SPRINT(" \\_________/           \\_________/ ");
}

// pretty print? :3
//       00%                 00%
//    _/ l1  \_____________/  r1 \_
//  /                               \
// |      ^                   b      |
// |    < + >     s l s     a + d    |
// |      v                   c      |
// |                                 |
// |     \ | /             \ | /     |
// |     - o -     ___     - o -     |
// |     / | \   /     \   / | \     |
// |            /       \            |
//  \_________/           \_________/

#include <CrcLib.h>
#include <communication/controller.hpp>
#include "util/print.hpp"

Controller controller;

void setup() {
    Serial.begin(115200);
    CrcLib::Initialize();
    Serial.println("Setup Done");
}

void loop() {
    CrcLib::Update();
    controller.update();

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

#include <CrcLib.h>
#include <communication/controller.hpp>

Controller controller;

void setup() {
    Serial.begin(115200);
    CrcLib::Initialize();
    Serial.println("Setup Done");
}

void loop() {
    CrcLib::Update();
    controller.update();

}
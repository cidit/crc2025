//https://robocrc.atlassian.net/wiki/spaces/AR/pages/437190657/GetDigitalInput
#include <CrcLib.h>

// Variable declaration
bool ButtonState;

void setup() {
    CrcLib::Initialize();

    CrcLib::SetDigitalPinMode(CRC_DIG_5, INPUT); //Set up DIO Port 5 as an Input

    /* 
    The rest of your setup code
    ...
     */
}
void loop() {
    CrcLib::Update();

    //The variable ButtonState will have the same value as DIO Port 5
    ButtonState = CrcLib::GetDigitalInput(CRC_DIG_5);
    
    /* 
    The rest of your looping code
    ...
    */
}
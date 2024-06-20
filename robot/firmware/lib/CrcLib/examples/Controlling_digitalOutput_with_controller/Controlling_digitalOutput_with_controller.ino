//How to manipulate digitalOutputs
//https://robocrc.atlassian.net/wiki/spaces/AR/pages/554270747/SetDigitalOutput

//How to access the state of the controller buttons
//https://robocrc.atlassian.net/wiki/spaces/AR/pages/560464350/CrcLib+How+to+access+the+remote+controller+buttons
#include <CrcLib.h>

void setup() {
    CrcLib::Initialize();

    //Set up DIO Port 1 as an Output
    CrcLib::SetDigitalPinMode(CRC_DIG_1, OUTPUT);
    
    /* 
    The rest of your setup code
    ...
     */
}
void loop() {
    CrcLib::Update();

    //Check Green Triangle button state (on a ps3 controller)
    if(CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP)==HIGH) //Button is pressed
    {
      CrcLib::SetDigitalOutput(CRC_DIG_1, LOW);   //Output 0V on pin
    }
    if(CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP)==LOW) //Button is not pressed
    {
      CrcLib::SetDigitalOutput(CRC_DIG_1, HIGH);   //Output 5V on pin
    }
    
    /* 
    The rest of your looping code
    ...
    */
}
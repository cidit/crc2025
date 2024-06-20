//How to manipulate digitalOutputs
//https://robocrc.atlassian.net/wiki/spaces/AR/pages/554270747/SetDigitalOutput

//How to access the state of the controller buttons
//https://robocrc.atlassian.net/wiki/spaces/AR/pages/560464350/CrcLib+How+to+access+the+remote+controller+buttons
#include <CrcLib.h>

signed char joystickValue;

void setup() {
    CrcLib::Initialize();
    
    CrcLib::InitializePwmOutput(CRC_PWM_12, 500, 2500);
    
    /* 
    The rest of your setup code
    ...
     */
}
void loop() {
    CrcLib::Update();

    joystickValue = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    CrcLib::SetPwmOutput(CRC_PWM_12, joystickValue );
    /* 
    The rest of your looping code
    ...
    */
}

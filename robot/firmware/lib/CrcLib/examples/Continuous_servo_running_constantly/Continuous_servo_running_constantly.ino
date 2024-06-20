//https://robocrc.atlassian.net/wiki/spaces/AR/pages/554303592/SetPwmOutput

#include <CrcLib.h>

void setup() {
    CrcLib::Initialize();

    CrcLib::InitializePwmOutput(CRC_PWM_5);
    CrcLib::SetPwmOutput(CRC_PWM_5,127);
    /* 
    The rest of your setup code
    ...
     */
}
void loop() {
    CrcLib::Update();

    /* 
    The rest of your looping code
    ...
    */
}

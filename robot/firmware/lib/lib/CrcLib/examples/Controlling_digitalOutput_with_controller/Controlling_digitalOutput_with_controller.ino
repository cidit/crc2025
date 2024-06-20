//How to manipulate digitalOutputs
//https://robocrc.atlassian.net/wiki/spaces/AR/pages/554270747/SetDigitalOutput

//How to access the state of the controller buttons
//https://robocrc.atlassian.net/wiki/spaces/AR/pages/560464350/CrcLib+How+to+access+the+remote+controller+buttons
#include <CrcLib.h>
#include <Encoder.h>

int joyL;


Encoder myEnc(CRC_DIG_2 , CRC_DIG_4 );

void setup() {

  Serial.begin(115200);
  CrcLib::Initialize();

  //Set up DIO Port 1 as an Output
  CrcLib::SetDigitalPinMode(CRC_DIG_1, OUTPUT);
  CrcLib::InitializePwmOutput(CRC_PWM_1);
  //CrcLib::SetDigitalPinMode(CRC_ENCO_B,INPUT);


  /* 
    The rest of your setup code
    ...
     */
}
long position = -999;
void loop() {
  CrcLib::Update();

  //Check Green Triangle button state (on a ps3 controller)
  // if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP) == HIGH)  //Button is pressed
  // {
  //   CrcLib::SetDigitalOutput(CRC_DIG_1, LOW);  //Output 0V on pin
  //   CrcLib::SetPwmOutput(CRC_PWM_1, 127);
  // }
  joyL = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);  //Button is not pressed
                                                          //Serial.println("JOYSTICK1_Y : "+String(joyL));
  CrcLib::SetDigitalOutput(CRC_DIG_1, HIGH);              //Output 5V on pin
  CrcLib::SetPwmOutput(CRC_PWM_1, joyL);

  volatile long newPos = myEnc.read();
  //Serial.println(newPos);
int i;

  if (newPos != position) {
    position = newPos;
    
    i++;
    Serial.println(position);
    Serial.println("lj  I : "+ String(i)  );
  }

  /* 
    The rest of your looping code
    ...
    */
}
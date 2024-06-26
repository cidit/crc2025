
#include <XBee.h>
#include <CrcLib.h>
#include "CrcRemoteState.h"
#include <Encoder.h>
#include <CrcLib/CrcBuzz.h>

// bool btnValue;
// byte joyL = 0;
// uint8_t JOYSTICK1_X;

void setup()
{
  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);

  Serial.begin(115200); // Open the serial Monitor at a 2000000 baud rate

  /*
    The rest of your setup code
    ...
     */
}

/**
 * 
*/
typedef struct gear_ratio_set {
  int small, big;  
};

int8_t pwm = 0;
bool clockwise = false;
Encoder wheel(CRC_ENCO_A, CRC_ENCO_B);
// Encoder angle(CRC_I2C_SDA, CRC_I2C_SCL);

void loop()
{
  CrcLib::Update();
  // if (millis() % 200 == 0)
  // {
  //   pwm += clockwise ? 1 : -1;
  // }
  // if (pwm == 127 || pwm == -128)
  // {
  //   clockwise = !clockwise;
  // }
  // CrcLib::SetPwmOutput(CRC_PWM_1, pwm / 2);
  // CrcLib::SetPwmOutput(CRC_PWM_2, pwm / 2);
  // Serial.println(pwm / 2);
  {
    // Serial.print("angle: " + String(angle.read()));
    Serial.print("\twheel: " + String(wheel.read()));
    Serial.println();
  }

  if (millis() % 5000 == 0)
  {
    // CrcLib::PlayTune(CrcUtility::TUNE_TEST, false);
  }

}

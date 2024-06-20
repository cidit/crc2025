
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
    // CrcLib::PlayTune(CrcUtility::TUNE_METRO, false);
  }

  //   uint8_t joy1_Y = map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y), -128,127,-63,63);
  //   uint8_t joy1_X = map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X), -128,127,-63,63);

  // y = map(x, 1, 50, 50, 1);
  // map(value, fromLow, fromHigh, toLow, toHigh)
  // JOYSTICK1_Y = map(ANALOG::JOYSTICK1_Y,-127,126,-63,63);
  // JOYSTICK1_X = map(ANALOG::JOYSTICK1_X,-127,126,-63,63);

  //   if (!CrcLib::IsCommValid())
  //   {
  //     CrcLib::MoveTank(0, 0, CRC_PWM_1, CRC_PWM_2);

  //   }
  //   else
  //   {
  //     CrcLib::MoveTank(joy1_X, joy1_Y, CRC_PWM_1, CRC_PWM_2);
  //   }
  // CrcLib::SetPwmOutput(CRC_PWM_1, joyL);

  // btnValue = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
  //  Serial.print(" JOYSTICKLX : " + String(joyL) + "   ");
  //  Serial.print("BOUTONX : " + String(btnValue));
  //  Serial.println();

  /*
    The rest of your looping code
    ...
    */
}
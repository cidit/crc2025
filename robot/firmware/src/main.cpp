
#include <XBee.h>
#include <CrcLib.h>
#include "CrcRemoteState.h"
#include <Encoder.h>
#include <CrcLib/CrcBuzz.h>
#include "drives/swerve.hpp"
#include <Decodeur.h>
#include "math/angles.hpp"

// bool btnValue;
// byte joyL = 0;
// uint8_t JOYSTICK1_X;

Decodeur decoder(&Serial);
int ticks_per_turn = 4700 / 5;

Encoder wheel_angle_e(CRC_ENCO_A, CRC_ENCO_B),
        wheel_speed_e(CRC_I2C_SDA, CRC_I2C_SCL);
RotaryEncoder wheel_angle_re(wheel_angle_e, ticks_per_turn),
              wheel_speed_re(wheel_speed_e, 0); // tpt UNKNOWN
Motor wheel_angle_m({CRC_PWM_1, 0, 0}), 
      wheel_speed_m({CRC_PWM_2, 0, 0}); // 0s mean its irrelevent in this impl
PrecisionMotor wheel_angle_pm(wheel_angle_m, wheel_angle_re), 
               wheel_speed_pm(wheel_speed_m, wheel_speed_re);
Swerve swerve(wheel_angle_pm, wheel_speed_pm);

void print_telemetry()
{

  // CrcLib::SetPwmOutput(CRC_PWM_1, pwm / 2);
  // CrcLib::SetPwmOutput(CRC_PWM_2, pwm / 2);
  // Serial.println(pwm / 2);
  {
    Serial.print("angle of roll: " + String(wheel_speed_e.read()));
    Serial.print("\tangle of wheel: " + String(wheel_angle_e.read()));
    Serial.println();
  }
}

enum Command : int
{
  AIM,
  UNKNOWN
};

int cmd_from_string(String c)
{
  if (c.equalsIgnoreCase("AIM"))
    return Command::AIM;
  return Command::UNKNOWN;
}

void setup()
{
  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);

  Serial.begin(115200); // Open the serial Monitor at a 2000000 baud rate
}

void loop()
{
  CrcLib::Update();
  decoder.refresh();

  switch (cmd_from_string(decoder.getCommandString()))
  {
  case AIM:
    swerve.aim_towards(Angle::from_deg(decoder.getArg(0)));
    break;

  case UNKNOWN:
    break;
  default:
    break;
  }

  if (millis() % 5000 == 0)
  {
    // CrcLib::PlayTune(CrcUtility::TUNE_TEST, false);
  }
}

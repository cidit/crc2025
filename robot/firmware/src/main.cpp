
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
sensors::RotaryEncoder wheel_angle_re(wheel_angle_e, ticks_per_turn),
    wheel_speed_re(wheel_speed_e, 0); // tpt UNKNOWN
drives::Motor wheel_angle_m({CRC_PWM_1, 0, 0}),
    wheel_speed_m({CRC_PWM_2, 0, 0}); // 0s mean its irrelevent in this impl
drives::PrecisionMotor wheel_angle_pm(wheel_angle_m, wheel_angle_re),
    wheel_speed_pm(wheel_speed_m, wheel_speed_re);
drives::Swerve swerve(wheel_angle_pm, wheel_speed_pm);

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

math::Angle x_y_to_angle(float x, float y)
{
  if (x == 0 || y == 0)
  {
    return math::Angle::zero();
  }
  auto rads = 1 / (tan(y / x));
  return math::Angle::from_rad(rads);
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
  swerve.loop();
  float yaw_x = map(float(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X)), 0.0, 255.0, -1.0, 1.0);
  float yaw_y = map(float(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)), 0.0, 255.0, -1.0, 1.0);
  x_y_to_angle(yaw_x, yaw_y);
  Serial.println(x_y_to_angle(yaw_x, yaw_y)._radians);
  /**
   * manette
   */

  switch (cmd_from_string(decoder.getCommandString()))
  {
  case AIM:
    wheel_angle_pm.set_target_angle(math::Angle::from_deg(decoder.getArg(0)));
    // swerve.aim_towards(math::Angle::from_deg(decoder.getArg(0)));
    break;

  case UNKNOWN:
    break;
  default:
    break;
  }

  if (millis() % 200 == 0)
  {
    Serial.print("wheel angle: ");
    Serial.println(wheel_angle_pm._encoder.getLast()._radians);
    Serial.print("wheel angle pid output: ");
    Serial.println(wheel_angle_pm._output);
    Serial.println();

    // CrcLib::PlayTune(CrcUtility::TUNE_TEST, false);
  }
}

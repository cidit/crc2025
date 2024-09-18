
#include <XBee.h>
#include <CrcLib.h>
#include "CrcRemoteState.h"
#include <Encoder.h>
#include <CrcLib/CrcBuzz.h>
#include "drives/swerve.hpp"
#include <Decodeur.h>
#include "math/angles.hpp"
#include "util/misc.hpp"
#include "util/cmd.hpp"
#include "util/needs_serial.hpp"
// #include <PID_RT.h>

Decodeur decoder(&Serial);
int ticks_per_turn = 4700 / 5;

Encoder a_e(CRC_ENCO_A, CRC_ENCO_B),
    b_e(CRC_I2C_SDA, CRC_I2C_SCL);
sensors::RotaryEncoder a_re(a_e, ticks_per_turn),
    b_re(b_e, 0); // tpt UNKNOWN
drives::Motor a_m({CRC_PWM_1, 0, 0}),
    b_m({CRC_PWM_2, 0, 0}); // 0s mean its irrelevent in this impl
drives::PrecisionMotor a_pm(a_m, a_re),
    b_pm(b_m, b_re);
drives::Swerve swerve(a_pm, b_pm);

void setup()
{

  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);

  Serial.begin(115200); 
  a_m.begin();
  a_m.set_speed(1);
}

void loop()
{
  CrcLib::Update();
  decoder.refresh();
  swerve.loop();
  float yaw_x = map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X), -128, 127, -127, 127);
  float yaw_y = -map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y), -128, 127, -127, 127);
  x_y_to_angle(yaw_x / 100, yaw_y / 100); // WHY /BY 100?
  // Serial.println(String(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X))+"    "+String(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)));

  // Serial.print("x" + String(yaw_x) + " y" + String(yaw_y) + " ");
  Serial.println(x_y_to_angle(yaw_x, yaw_y)._radians);

  auto current_wheel_angle = math::Angle::zero();
  a_re.sample(current_wheel_angle);

  switch (cmd_from_string(decoder.getCommandString()))
  {
  case AIM:
    a_pm.set_target_angle(math::Angle::from_deg(decoder.getArg(0)));
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
    Serial.println(a_pm._encoder.getLast()._radians);
    Serial.print("wheel angle pid output: ");
    Serial.println(a_pm._output);
    Serial.println();

    // CrcLib::PlayTune(CrcUtility::TUNE_TEST, false);
  }
}

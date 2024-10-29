#include "util/misc.hpp"
#include "util/needs_serial.hpp"
// #include <CrcLib.h>
// #include "CrcRemoteState.h"
#include <Encoder.h>
// #include <CrcLib/CrcBuzz.h>
#include "drives/swerve.hpp"
#include <Decodeur.h>
// #include <XboxSeriesXControllerESP32_asukiaaa.hpp>
#include "math/angles.hpp"
#include "util/cmd.hpp"
#include "util/angles.hpp"

// #define ENCODER_DO_NOT_USE_INTERRUPTS

Decodeur decoder(&Serial);
float ticks_per_turn = 537.7; // 4700 / 5;

// Encoder a_e(CRC_ENCO_A, CRC_ENCO_B),
//     b_e(CRC_I2C_SDA, CRC_I2C_SCL);
// sensors::RotaryEncoder a_re(a_e, ticks_per_turn),
//     b_re(b_e, 0); // tpt UNKNOWN
// drives::Motor a_m({CRC_PWM_1, 0, 0}),
//     b_m({CRC_PWM_2, 0, 0}); // 0s mean its irrelevent in this impl
// drives::PrecisionMotor a_pm(a_m, a_re),
//     b_pm(b_m, b_re);
// drives::Swerve swerve(a_pm, b_pm);

// Encoder a_e(33, 32);

// sensors::RotaryEncoder a_re(a_e, ticks_per_turn);
// drives::Motor a_m({2, 0, 0});
// drives::PrecisionMotor a_pm(a_m, a_re);
// drives::Swerve swerve(a_pm, b_pm);

#include <ESP32Encoder.h> // https://github.com/madhephaestus/ESP32Encoder.git
#include "sensors/rotary_encoder.hpp"

sensors::RotaryEncoder re{34, 35, ticks_per_turn};
drives::Motor a_m({0, 0, 33});

void setup()
{
  Serial.begin(115200);
  NeedsSerial::wait_for_serial(500);

#if defined(USING_CRCLIB)
  CrcLib::Initialize();

  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);
#endif

  a_m.begin();
  a_m.set_speed(0.1);
}

void loop()
{
  decoder.refresh();
  re.poll();

#if defined(USING_CRCLIB)
  CrcLib::Update();
  // swerve.loop();
  float x = map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X), -128, 127, -127, 127);
  float y = -map(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y), -128, 127, -127, 127);
  // Serial.println(x_y_to_angle(x, y)._radians);
#endif

  NS({
    Serial.println(
        " re: " + String(re.getLast()._radians));
  })

  // auto current_wheel_angle = math::Angle::zero();
  // a_re.sample(current_wheel_angle);

  if (!decoder.isAvailable())
    return;
  auto ack = true;
  switch (cmd_from_string(decoder.getCommandString()))
  {
  case Command::SPD:
  {

    /**
     * sets the speed of the motor, value from 0-255, passes the value directly
     */
    auto a1 = decoder.getArg(0);
    // a_m.set_speed(a1);
    analogWrite(33, a1);
    Serial.println("NEW SPEED: " + String(a1));
    break;
  }
  default:
  {
    ack = false;
    break;
  }
  }
  Serial.println(ack ? "ACK" : "NAK");
}

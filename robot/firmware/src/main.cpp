#include <CrcLib.h>
#include <Encoder.h>
#include <PID_RT.h>
#include <Decodeur.h>
#include <utility>

// TODO: plot Processed Variable (of angle_pid graph) with serial plotter

const double SETPOINT = 0;
float target_angle = 0;
bool motors_enabled = false;

Decodeur decodeur(&Serial);
PID_RT angle_pid(SETPOINT, 500, 0, 0);

namespace controller
{

  struct Joystick
  {
    float x, y;
  };

  struct Controller
  {
    Joystick translation, rotation;
    bool cross, square, circle, triangle;
  };

  float logarithmize(float x)
  {
    // less agressive
    return (pow(x, 3) * 0.7) + (x * 0.3);
    // // more agressive
    // return pow(x, 3);
  }

  float i8_to_ratio(int8_t i)
  {
    float f = i;
    return i > 0 ? f / 127 : f / 128;
  }

  Controller get_controller_state()
  {
    if (!CrcLib::IsCommValid())
    {
      return {{0, 0}, {0, 0}, false, false, false, false};
    }
    Joystick trans = {
        logarithmize(i8_to_ratio(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X))),
        -logarithmize(i8_to_ratio(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y))),
    };
    Joystick rot = {
        i8_to_ratio(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X)),
        -i8_to_ratio(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y)),
    };

    return {
        trans, rot, false, false, false, false};
  }

}

float clamp_angle_0_to_1(float angle_to_clamp)
{
  auto wrapped = fmod(angle_to_clamp, 1);
  return wrapped < 0 ? 1 - wrapped : wrapped;
}

float xy_to_angle(float x, float y) {

    auto angle_centered_on_zero = -atan2(y, x)/(2*PI);
    return angle_centered_on_zero > 0? angle_centered_on_zero: 1+angle_centered_on_zero;
}

void apply_cmds()
{
  if (!decodeur.isAvailable())
  {
    return;
  }
  bool ack = true;
  switch (decodeur.getCommand())
  {
  case 'A':
  {
    // change the angle
    auto newA = decodeur.getArg(0);
    target_angle = clamp_angle_0_to_1(newA);
    break;
  }
  case 'K':
  {
    if (decodeur.getArgCount() != 3)
    {
      Serial.println("incorrect num of args");
      ack = false;
      break;
    }
    auto p = decodeur.getArg(0),
         i = decodeur.getArg(1),
         d = decodeur.getArg(2);
    angle_pid.setK(p, i, d);
    break;
  }
  case 'M':
  {
    motors_enabled = !motors_enabled;
    break;
  }
  default:
    ack = false;
  }
  Serial.println(ack ? '!' : '?');
}

/**
 * reads the angle as a ratio
 */
float read_angle_as_ratio()
{
  const double PULSE_MAX = 4160; // in micros
  const double PULSE_MIN = 1;    // in micros
  auto pulse = pulseIn(CRC_PWM_12, HIGH);
  auto normalized = constrain(pulse - PULSE_MIN, 0.0, PULSE_MAX);
  auto ratio = normalized / PULSE_MAX;
  // we do 1 - ratio because the encoder reverses clockwise and counter clockwise
  return 1 - ratio;
}

enum class Direction
{
  CLOCKWISE,
  COUNTERCLOCKWISE
};

String direction_to_str(Direction dir)
{
  return dir == Direction::CLOCKWISE ? "CLCK" : "CNTR";
}

void setup()
{
  Serial.begin(115200);
  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  // Pinmode pour le PWM
  pinMode(CRC_PWM_12, INPUT);
  angle_pid.setPropOnError();
  angle_pid.setOutputRange(-125, 125);
  angle_pid.setInterval(1);
  angle_pid.start();
}

float cosine_optimization(float speed, float angle_travel)
{
  /**
   * cosine optimization is used to reduce the speed of the swerve drive
   * proportionally with it's distance with the expected angle. i found it in
   * a chiefdelphi paper i lost the trace of. the idea is the closer the angle
   * you give to cos is to 90, the closer cos's output is gonna be close to 0,
   * and the closer the angle you give it is close to 0, the closer the output
   * is gonna be to 1.
   *
   * @param angle_travel is clamped between -1/4 turns and +1/4 turns
   */
  auto angle_travel_true = constrain(angle_travel, -0.25, +0.25);
  auto angle_rad = angle_travel_true * 2 * PI;
  // we remove 0.001 because of floating point bullshit making it not come out as 0
  angle_rad += angle_rad > 0 ? -0.001 : +0.001;
  auto optimized_speed = cos(angle_rad);
  return optimized_speed;
}

struct ora_result
{
  float shortest;
  bool reversed;
};

ora_result optimized_travel(float current, float target)
{
  auto zeroed = target - current;
  auto shortest = zeroed;
  if (zeroed > 0.5)
  {
    shortest = zeroed - 1;
  }
  else if (zeroed < -0.5)
  {
    shortest = zeroed + 1;
  }

  auto canOraOptimize = abs(shortest) > 0.25;
  if (canOraOptimize)
  {
    // antipodal optimization is the better name
    shortest += shortest > 0 ? -0.5 : +0.5;
  }

  return {shortest, canOraOptimize};
}

#if false
// guillaume's fucking thinger
void loop() {
  CrcLib::Update();
  CrcLib::SetPwmOutput(CRC_PWM_1, -127);
}
#else

void loop()
{
  CrcLib::Update();
  decodeur.refresh();
  apply_cmds();

  auto controller_state = controller::get_controller_state();
  if (abs(controller_state.rotation.x) >= 0.1 && abs(controller_state.rotation.y) >= 0.1) {
    target_angle = xy_to_angle(controller_state.rotation.x, controller_state.rotation.y);
  }

  auto current_angle = read_angle_as_ratio();

  auto ora_result = optimized_travel(current_angle, target_angle);

  auto direction = ora_result.shortest > 0
                       ? Direction::CLOCKWISE
                       : Direction::COUNTERCLOCKWISE;

  if (angle_pid.compute(abs(ora_result.shortest)) && motors_enabled)
  {
    // Serial.print("computed");
    auto angle_output = angle_pid.getOutput();

    switch (direction)
    {
    case Direction::CLOCKWISE:
    {
      CrcLib::SetPwmOutput(CRC_PWM_1, -angle_output);
      CrcLib::SetPwmOutput(CRC_PWM_2, angle_output);
      break;
    }
    case Direction::COUNTERCLOCKWISE:
    {
      CrcLib::SetPwmOutput(CRC_PWM_1, angle_output);
      CrcLib::SetPwmOutput(CRC_PWM_2, -angle_output);
      break;
    }
    }
  }

  if (!motors_enabled)
  {
    CrcLib::SetPwmOutput(CRC_PWM_1, 0);
    CrcLib::SetPwmOutput(CRC_PWM_2, 0);
  }
  static long last_print = 0;
  if (millis() - last_print > 1000)
  {
    last_print = millis();

    auto atantrucbin = -atan2(controller_state.rotation.y, controller_state.rotation.x)/(2*PI);
    Serial.println(
        "curr: " + String(current_angle) +
        "  targ: " + String(target_angle) +
        // "  taze: " + String(zeroed) +
        "  opti: " + String(ora_result.shortest) +
        "  [p " + String(angle_pid.getKp()) +
        "|i " + String(angle_pid.getKi()) +
        "|d " + String(angle_pid.getKd()) +
        "]" +
        "  out: " + String(angle_pid.getOutput()) +
        "  [tx " + controller_state.translation.x +
        "|ty " + controller_state.translation.y +
        "|rx " + controller_state.rotation.x +
        "|ry " + controller_state.rotation.y +
        "]" +
        "  rot: " + (atantrucbin > 0? atantrucbin: 1+atantrucbin) +
        "");
  }
}

#endif
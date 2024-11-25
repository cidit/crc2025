#include <CrcLib.h>
#include <Encoder.h>
#include <speedCalculation.hpp>
#include <controller.hpp>
#include "math/vectors.hpp"
using math::cartesian::Vec2D;

<<<<<<< HEAD
//----- Variables -----
double joyAngle = 0;
double joySpeed = 1;

Controller ctrl;
SwerveModule swerveA;

int printDelai = 500;
int printTimer = millis();
=======
// TODO: plot Processed Variable (of angle_pid graph) with serial plotter

const double SETPOINT = 0;
float target_angle_as_ratio = 0;
bool motors_enabled = false;

Decodeur decodeur(&Serial);
PID_RT angle_pid;

float clamp_angle_0_to_1(float angle_to_clamp)
{
  auto wrapped = fmod(angle_to_clamp, 1);
  return wrapped < 0 ? 1 - wrapped : wrapped;
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
    target_angle_as_ratio = clamp_angle_0_to_1(newA);
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
  const double PULSE_MIN = 1; // in micros
  auto pulse = pulseIn(CRC_PWM_12, HIGH);
  auto normalized = constrain(pulse - PULSE_MIN, 0.0, PULSE_MAX);
  auto ratio = normalized / PULSE_MAX;
  // we do 1 - ratio because the encoder reverses clockwise and counter clockwise
  return 1-ratio;
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
>>>>>>> ec5de45 (v1 with pid fooken works bb)

//----- Main Program ----------------------------------------------------------------------
void setup(){
  Serial.begin(115200);

  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  // Pinmode pour le PWM
  pinMode(CRC_PWM_12, INPUT);
<<<<<<< HEAD

  //Initialisation des modules Swerve
  swerveA.init(0.002, 0.00, 0.0);

  Serial.println("Setup Done");
}

void loop(){
  CrcLib::Update();
  ctrl.update();


  Vec2D vector = swerveA.calculate(ctrl.get_left_joy().angleDeg, ctrl.get_left_joy().norm);
  swerveA.setMotorPowers(vector);

  if (millis() - printTimer >= printDelai) {
    printTimer = millis();

    Serial.println("Angle: " + String(ctrl.get_left_joy().angleRad));
    Serial.println("Norm : " + String(ctrl.get_left_joy().norm));
    Serial.println("X    : " + String(ctrl.get_left_joy().x));
    Serial.println("Y    : " + String(ctrl.get_left_joy().y));
    Serial.println();
  }

}
=======
  angle_pid.setK(300, 0, 0);
  angle_pid.setPropOnError();
  angle_pid.setOutputRange(-125, 125);
  angle_pid.setInterval(1);
  angle_pid.start();
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

  auto current_angle = read_angle_as_ratio();

  auto ora_result = optimized_travel(current_angle, target_angle_as_ratio);

  auto direction = ora_result.shortest > 0
                       ? Direction::CLOCKWISE
                       : Direction::COUNTERCLOCKWISE;

  auto distance = abs(ora_result.shortest);

  if (angle_pid.compute(abs(ora_result.shortest)) && motors_enabled)
  {
    // Serial.print("computed");
    auto output = angle_pid.getOutput();

    switch (direction)
    {
    case Direction::CLOCKWISE:
    {
      CrcLib::SetPwmOutput(CRC_PWM_1, -output);
      CrcLib::SetPwmOutput(CRC_PWM_2, output);
      break;
    }
    case Direction::COUNTERCLOCKWISE:
    {
      CrcLib::SetPwmOutput(CRC_PWM_1, output);
      CrcLib::SetPwmOutput(CRC_PWM_2, -output);
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
    Serial.println(
        "curr: " + String(current_angle) +
        "  taag: " + String(target_angle_as_ratio) +
        // "  taze: " + String(zeroed) +
        "  opti: " + String(ora_result.shortest) +
        "  dist: " + String(distance) +
        "  dir: " + String(direction_to_str(direction)) +
        "  [p " + String(angle_pid.getKp()) + "|i " + String(angle_pid.getKi()) + "|d " + String(angle_pid.getKd()) + "]" +
        "  out: " + String(angle_pid.getOutput()) +
        "");
  }
}

#endif
>>>>>>> ec5de45 (v1 with pid fooken works bb)

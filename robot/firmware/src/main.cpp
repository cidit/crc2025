/*
  Main program for robot control

  
*/
#include <CrcLib.h>
#include <Encoder.h>
#include <swerveModule.hpp>
#include <controller.hpp>
#include "math/vectors.hpp"
using math::cartesian::Vec2D;

//----- Variables -----
double joyAngle = 0;
double joySpeed = 1;

Controller ctrl;
SwerveModule swerveA;

int printDelai = 500;
int printTimer = millis();

//----- Main Program ----------------------------------------------------------------------
void setup(){
  Serial.begin(115200);

  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  // Pinmode pour le PWM
  pinMode(CRC_PWM_12, INPUT);

  //Initialisation des modules Swerve
  swerveA.init(0.002, 0.00, 0.0);

  Serial.println("Setup Done");
}

void loop(){
  CrcLib::Update();
  ctrl.update();

  //ctrl.get_left_joy().angleDeg ctrl.get_left_joy().norm
  Vec2D vector = swerveA.calculateRad(7*M_PI/4, 1);
  Serial.println("X" + String(vector.x()));
  Serial.println("Y" + String(vector.y()));
  //swerveA.setMotorPowers(vector);

  // if (millis() - printTimer >= printDelai) {
  //   printTimer = millis();

  //   Serial.println("Angle: " + String(ctrl.get_left_joy().angleRad));
  //   Serial.println("Norm : " + String(ctrl.get_left_joy().norm));
  //   Serial.println("X    : " + String(ctrl.get_left_joy().x));
  //   Serial.println("Y    : " + String(ctrl.get_left_joy().y));
  //   Serial.println();
  // }

}




//------------------------------------------------------------------------------------------------------------
float clamp_angle_0_to_1(float angle_to_clamp)
{
  auto wrapped = fmod(angle_to_clamp, 1);
  return wrapped < 0 ? 1 - wrapped : wrapped;
}

float xy_to_angle(float x, float y) {

    auto angle_centered_on_zero = -atan2(y, x)/(2*PI);
    return angle_centered_on_zero > 0? angle_centered_on_zero: 1+angle_centered_on_zero;
}

// void apply_cmds()
// {
//   if (!decodeur.isAvailable())
//   {
//     return;
//   }
//   bool ack = true;
//   switch (decodeur.getCommand())
//   {
//   case 'A':
//   {
//     // change the angle
//     auto newA = decodeur.getArg(0);
//     target_angle = clamp_angle_0_to_1(newA);
//     break;
//   }
//   case 'K':
//   {
//     if (decodeur.getArgCount() != 3)
//     {
//       Serial.println("incorrect num of args");
//       ack = false;
//       break;
//     }
//     auto p = decodeur.getArg(0),
//          i = decodeur.getArg(1),
//          d = decodeur.getArg(2);
//     angle_pid.setK(p, i, d);
//     break;
//   }
//   case 'M':
//   {
//     motors_enabled = !motors_enabled;
//     break;
//   }
//   default:
//     ack = false;
//   }
//   Serial.println(ack ? '!' : '?');
// }

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

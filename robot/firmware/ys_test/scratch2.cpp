#include <PID_RT.h>
#include <CrcLib.h>
// #include <Encoder.h>

#include "math/angles.hpp"


// PID parameters for angle control
double input = 0;
double output = 0;
double oppAngleWrap;
double currentAngleWrap;
double angleFromTarget;
double oppAngleFromTarget;
double rPower;


math::Angle target_angle = math::Angle::zero();

// Initialize PID controller
PID_RT pid;

// Variables for motor control
double motorSpeedA = 0;
double motorSpeedB = 0;
double currentAngle = 0;
unsigned long lastTime = 0;
const double timeInterval = 100;  // Time interval in milliseconds

// Encoder Enco_MotorA(CRC_ENCO_A, CRC_ENCO_B);
// Encoder Enco_MotorB(CRC_I2C_SDA, CRC_I2C_SCL);

// Gear ratios for the differential swerve module
const double gearRatioA = 5.0;  // Replace with your actual gear ratio
const double gearRatioB = 5.0;  // Replace with your actual gear ratio

double angleWrap(double x) {
  x = fmod(x + 180, 360);
  Serial.println(x);
  if (x < 0)
    x += 360;
  return x - 180;
}
// double angleOptimisation(double radians) {

//   if ((radians > 90) && (radians < 270)) {
//     radians -= 180;
//   }
//   if (radians > 270) {
//     radians -= 360;
//   }
//   return radians;
// }
void setup() {
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  pinMode(CRC_PWM_12, INPUT);
  //CrcLib::InitializePwmOutput(CRC_PWM_12);
  Serial.begin(112500);

  // Initialize PID controller
  //pid.start();
  pid.setPoint(0);
  pid.setOutputRange(-50, 50);  // PWM range
  // pid.setReverse(true);
  // pid.setInterval(5);
  pid.setK(1, 0, 0);  // a modifier overshoot un peu beaucoup
}

void loop() {
  CrcLib::Update();
  
  // unsigned long now = millis();
  // double timeChange = (double)(now - lastTime);
  auto currentAngle = math::Angle::from_ratio(pulseIn(CRC_PWM_12, HIGH) / 4195.0 * 360);

  // oppAngleWrap = angleWrap(currentAngle + 180);
  // angleFromTarget = angleWrap(setpoint - currentAngle);
  // oppAngleFromTarget = angleWrap(setpoint - oppAngleWrap);
  auto travel = math::Angle::travel(currentAngle, target_angle);

  //Serial.println("currentAngle: "+String(currentAngle._radians) + "\ttravel: "+ String(travel));

  // if (abs(angleFromTarget) > abs(oppAngleFromTarget)) {
  //   rPower = pid.compute(oppAngleFromTarget);
  // } else{ 
  //   rPower = pid.compute(angleFromTarget);
  // }

  // if (!CrcLib::IsCommValid()) {/ 4195.0 * 360
  //   CrcLib::SetPwmOutput(CRC_PWM_2, 0);
  //   CrcLib::SetPwmOutput(CRC_PWM_1, 0);
  //   pid.stop();
  // }
  // double motorPositionA = Enco_MotorA.read() / gearRatioA;
  // double motorPositionB = Enco_MotorB.read() / gearRatioB;
  //pid.compute(input);
  // output = pid.getOutput();
  //Serial.println(output);

  // if (timeChange >= timeInterval) {
  //   lastTime = now;

  // Read the current angle from the absolute encoder

  // Update the PID input

  // Compute the PID output

  // Read the motor positions from the encoders

  // Calculate the motor speeds based on the PID output and motor positions
  // motorSpeedA = -output;
  // motorSpeedB = output;

  // // // Set the motor speeds
  // setMotorSpeeds(motorSpeedA, motorSpeedB);

  // // Print current angle and motor speeds for debugging
  Serial.print("Current Angle: ");
  Serial.println(output);
  // Serial.print(" degrees, PID Output: ");
  // Serial.println(output);
  // Serial.print(", Motor Speed A: ");
  // Serial.print(motorSpeedA);
  // Serial.print(", Motor Speed B: ");
  // Serial.println(motorSpeedB);
  // }
}

void setMotorSpeeds(double speedA, double speedB) {
  // Convert speeds to PWM values (assuming a linear relationship)
  int pwmValueA = speedA;
  int pwmValueB = speedB;

  CrcLib::SetPwmOutput(CRC_PWM_1, pwmValueA);
  CrcLib::SetPwmOutput(CRC_PWM_2, pwmValueB);
}
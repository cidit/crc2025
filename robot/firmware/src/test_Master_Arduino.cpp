#include <PID_RT.h>
#include <CrcLib.h>
#include <Encoder.h>

#define encoAbs 45
// Define the pins for the motors

// PID parameters for angle control
double setpoint = 70;  // Target angle in degrees
double input = 0;
double output = 0;
double oppAngleWrap;
double currentAngleWrap;
double angleFromTarget;
double oppAngleFromTarget;
bool rPower;

// Initialize PID controller
PID_RT pid;

// Variables for motor control
double motorSpeedA = 0;
double motorSpeedB = 0;
double currentAngle = 0;
unsigned long lastTime = 0;
const double timeInterval = 100;  // Time interval in milliseconds
Encoder Enco_MotorA(CRC_ENCO_A, CRC_ENCO_B);
Encoder Enco_MotorB(CRC_I2C_SDA, CRC_I2C_SCL);

// Gear ratios for the differential swerve module
const double gearRatioA = 5.0;  // Replace with your actual gear ratio
const double gearRatioB = 5.0;  // Replace with your actual gear ratio

double angleWrap(double x) {
  x = fmod(x + 180, 360);
  if (x < 0)
    x += 360;
  return x - 180;
}

void setup() {
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_1);
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  pinMode(CRC_PWM_8, INPUT);
  //CrcLib::InitializePwmOutput(CRC_PWM_12);
  Serial.begin(9600);

  // Initialize PID controller
  pid.setPoint(setpoint);
  pid.setOutputRange(-50, 50);  // PWM range
  pid.setK(0.75, 0.7, 0.002);   // a modifier overshoot un peu beaucoup
}


void loop() {
  CrcLib::Update();
  pid.start();
  pid.setPoint(setpoint);
  // unsigned long now = millis();
  // double timeChange = (double)(now - lastTime);
  currentAngle = pulseIn(CRC_PWM_8, HIGH) / 4195.0 * 360;

  oppAngleWrap = angleWrap(currentAngle - 180);
  angleFromTarget = angleWrap(setpoint - currentAngle);
  oppAngleFromTarget = angleWrap(setpoint - oppAngleWrap);

  if (abs(angleFromTarget) > abs(oppAngleFromTarget)) {
    rPower = pid.compute(oppAngleFromTarget);
  } else {
    rPower = pid.compute(angleFromTarget);
  }

  if (rPower) {
    output = pid.getOutput();
    // // Set the motor speeds
    setMotorSpeeds(output, -output);
  }

  // Print current angle and motor speeds for debugging
  Serial.print("Current Angle: ");
  Serial.println(output);
}

void setMotorSpeeds(double speedA, double speedB) {
  // Convert speeds to PWM values (assuming a linear relationship)
  int pwmValueA = speedA;
  int pwmValueB = speedB;

  CrcLib::SetPwmOutput(CRC_PWM_1, pwmValueA);
  CrcLib::SetPwmOutput(CRC_PWM_2, pwmValueB);
}

//https://github.com/NathanT18172/ftc-diff-swerve/blob/master/DriveModule.java copy translate 

#include <Servo.h>
#include <Encoder.h>

// Define constants
const int RIGHT_TOP_MOTOR_PIN = 9;
const int RIGHT_BOTTOM_MOTOR_PIN = 10;
const int LEFT_TOP_MOTOR_PIN = 11;
const int LEFT_BOTTOM_MOTOR_PIN = 12;
const double MAX_MOTOR_POWER = 127.0;
const double ALLOWED_MODULE_ORIENTATION_ERROR = 5.0;
const double ANGLE_OF_MAX_MODULE_ROTATION_POWER = 90.0;
const double ROT_ADVANTAGE = 1.0;
const double DEGREES_PER_TICK = 0.1; // Example value, adjust as needed
const double CM_PER_TICK = 0.1; // Example value, adjust as needed

// Define motor and encoder objects
Servo motor1;
Servo motor2;
Encoder encoder1(2, 3); // Example pins for encoder
Encoder encoder2(4, 5); // Example pins for encoder

// Define variables
double currentAngle = 0;
double setpointAngle = 100;
double setpointSpeed = 50; // Desired speed
bool reversed = false;
bool takingShortestPath = false;
double distanceTraveled = 0;
double lastMotor1Encoder = 0;
double lastMotor2Encoder = 0;

void setup() {
  Serial.begin(9600);

  // Attach motors to pins
  motor1.attach(RIGHT_TOP_MOTOR_PIN);
  motor2.attach(RIGHT_BOTTOM_MOTOR_PIN);

  // Initialize motors
  motor1.writeMicroseconds(1500); // Neutral position
  motor2.writeMicroseconds(1500); // Neutral position
}

void loop() {
  // Example direction vector for rotation
  double directionX = 1.0;
  double directionY = 0.0;
  bool fieldCentric = true;

  rotateModule(directionX, directionY, fieldCentric);

  delay(100); // Loop delay
}

void rotateModule(double directionX, double directionY, bool fieldCentric) {
  double convertedRobotHeading = getRobotHeading(); // Simplified for Arduino

  double directionFCX = directionX; // Simplified for Arduino
  double directionFCY = directionY; // Simplified for Arduino

  if (reversed) {
    directionFCX = -directionFCX;
    directionFCY = -directionFCY;
    directionX = -directionX;
    directionY = -directionY;
  }

  double powerVecX = 0;
  double powerVecY;
  if (fieldCentric) {
    powerVecY = getPivotComponent(directionFCX, directionFCY, getCurrentOrientation());
  } else {
    powerVecY = getPivotComponent(directionX, directionY, getCurrentOrientation());
  }
  setMotorPowers(powerVecX, powerVecY);

  Serial.print("Power Vector: ");
  Serial.print(powerVecX);
  Serial.print(", ");
  Serial.println(powerVecY);
  Serial.print("Current orientation: ");
  Serial.println(getCurrentOrientation());
}


//replace with scratch3
double getPivotComponent(double targetVecX, double targetVecY, double currentAngle) {
  double targetAngle = atan2(targetVecY, targetVecX) * 180 / PI;
  double angleDiff = abs(targetAngle - currentAngle);

  if (angleDiff > 110) {
    if (!takingShortestPath) {
      reversed = !reversed;
    }
    takingShortestPath = true;
  } else {
    takingShortestPath = false;
  }

  if (angleDiff < ALLOWED_MODULE_ORIENTATION_ERROR) {
    return 0;
  } else if (angleDiff > ANGLE_OF_MAX_MODULE_ROTATION_POWER) {
    return (targetAngle > currentAngle) ? ROT_ADVANTAGE : -ROT_ADVANTAGE;
  } else {
    return (targetAngle > currentAngle) ? angleDiff / ANGLE_OF_MAX_MODULE_ROTATION_POWER * ROT_ADVANTAGE : -angleDiff / ANGLE_OF_MAX_MODULE_ROTATION_POWER * ROT_ADVANTAGE;
  }
}

void setMotorPowers(double powerVecX, double powerVecY) {
  double motor1Power = constrain(powerVecX + powerVecY, -MAX_MOTOR_POWER, MAX_MOTOR_POWER);
  double motor2Power = constrain(powerVecX - powerVecY, -MAX_MOTOR_POWER, MAX_MOTOR_POWER);

  motor1.writeMicroseconds(map(motor1Power, -MAX_MOTOR_POWER, MAX_MOTOR_POWER, 1000, 2000));
  motor2.writeMicroseconds(map(motor2Power, -MAX_MOTOR_POWER, MAX_MOTOR_POWER, 1000, 2000));

  Serial.print("Motor 1 Power: ");
  Serial.println(motor1Power);
  Serial.print("Motor 2 Power: ");
  Serial.println(motor2Power);
}

void resetEncoders() {
  encoder1.write(0);
  encoder2.write(0);
}


//read ABS_encoder
double getCurrentOrientation() {
  double rawAngle = (encoder2.read() + encoder1.read()) * DEGREES_PER_TICK;
  return rawAngle;
}

void updateTracking() {
  double currentMotor1Encoder = encoder1.read();
  double currentMotor2Encoder = encoder2.read();

  double motor1Change = currentMotor1Encoder - lastMotor1Encoder;
  double motor2Change = currentMotor2Encoder - lastMotor2Encoder;

  if (reversed) {
    distanceTraveled -= (motor1Change - motor2Change) / 2.0 * CM_PER_TICK;
  } else {
    distanceTraveled += (motor1Change - motor2Change) / 2.0 * CM_PER_TICK;
  }

  lastMotor1Encoder = currentMotor1Encoder;
  lastMotor2Encoder = currentMotor2Encoder;

  Serial.print("Motor 1 Encoder: ");
  Serial.println(currentMotor1Encoder);
  Serial.print("Motor 2 Encoder: ");
  Serial.println(currentMotor2Encoder);
}

void resetDistanceTraveled() {
  distanceTraveled = 0;
  lastMotor1Encoder = encoder1.read();
  lastMotor2Encoder = encoder2.read();
}

double getDistanceTraveled() {
  return distanceTraveled;
}

double getRobotHeading() {
  // Simplified for Arduino, replace with actual heading calculation
  return 0;
}


// math::Angle x_y_to_angle(float x, float y)
// {
//     if (y == 1 && x == 0)
//     {
//         return math::Angle::zero();
//     }
//     if (x == 0 && y > 0)
//     {
//         return math::Angle::from_rad(PI / 2);
//     }
//     if (x == 0 && y < 1)
//     {
//         return math::Angle::from_rad(3 * PI / 2);
//     }
//     if (x > 0 && y == 1)
//     {
//         return math::Angle::zero();
//     }
//     if (x < 0 && y == 1)
//     {
//         return math::Angle::from_rad(PI);
//     }
//     if ((x != 0) && y < 1)
//     {
//         return math::Angle::from_rad(atan2(y, x) + (2 * PI));
//     }
//     return math::Angle::from_rad(atan2(y, x));
// }


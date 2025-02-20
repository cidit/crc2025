#include <Arduino.h>
#include <CrcLib.h>

Servo servo_manipulator_1; // PWM11
Servo servo_manipulator_2; // PWM12
Servo servo_manipulator_3; // PWM3

void setup()
{
    CrcLib::Initialize();
    Serial.begin(115200);
    servo_manipulator_1.attach(CRC_PWM_3, 1000, 2000);
    servo_manipulator_2.attach(CRC_PWM_4, 1000, 2000);
    servo_manipulator_3.attach(CRC_PWM_2, 1000, 2000);
}

void loop()
{
    CrcLib::Update();
    Serial.println("heartbeat");
    servo_manipulator_1.write(2000);
    servo_manipulator_2.write(2000);
    servo_manipulator_3.write(2000);
}
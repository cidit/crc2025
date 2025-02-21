#include <Arduino.h>
#include <CrcLib.h>
#include <sensors/pwm_rotary_enc.hpp>
#include <util/timer.hpp>

Servo servo_manipulator_1; // PWM11
Servo servo_manipulator_2; // PWM12
Servo servo_manipulator_3; // PWM3

Timer poll_timer(1000 / 20);

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco_left(CRC_DIG_1, MAX_PULSE_LEN, -1.07, poll_timer);
PwmRotaryEncoder pwm_enco_right(CRC_DIG_2, MAX_PULSE_LEN, -2.10, poll_timer);

void setup()
{
    CrcLib::Initialize();
    Serial.begin(115200);
    pwm_enco_left.begin();
    pwm_enco_right.begin();
}

void loop()
{
    CrcLib::Update();
    poll_timer.update(millis());

    if (poll_timer.is_time())
    {
        pwm_enco_left.update();
        pwm_enco_right.update();
        Serial.println("heartbeat");
        Serial.println(pwm_enco_left.getLast().rads);
        Serial.println(pwm_enco_right.getLast().rads);
        Serial.println();
    }
}
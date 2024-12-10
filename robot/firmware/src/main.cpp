#include <Arduino.h>
#include <Decodeur.h>
#include <CrcLib.h>
#include <PID_v1.h>
#include "drives/precision_motor.hpp"
#include "drives/motor.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"

//-------------------------- DEFINES -----------------------------
#define TICKS_117 1425.1  //Bras
#define TICKS_312 537.7 
#define TICKS_1150 145.1 //Lanceur, swerve


//-------------------------- VARIABLES -----------------------------
double in, out, s=0;
auto target_angle = math::Angle::zero();


//-------------------------- OBJECTS -----------------------------
Decodeur cmdl(&Serial);
drives::Motor motor({CRC_PWM_1, 0, 0}, false);
sensors::GobuildaRotaryEnco re(CRC_ENCO_A, CRC_ENCO_B, 537.7);
drives::PrecisionMotor pm(motor, re, 1.0, 0.0, 0.0, 5);
Timer print_timer(500); // ms


//-------------------------- FUNCTIONS -----------------------------
void update_cmd(){
  if (!cmdl.isAvailable())
  {
    return;
  }

  auto ack = true;
  switch (toupper(cmdl.getCommand()))
  {
  case 'S':
  {
    if (cmdl.getArgCount() < 1)
    {
      ack = false;
      break;
    }
    auto speed = cmdl.getArg(0);
    Serial.println("setting speed to: " + String(speed));
    motor.set_speed(speed);
    break;
  }
  case 'K': {
    if (cmdl.getArgCount() < 3) {
      ack = false;
      break;
    }
    double p = cmdl.getArg(0), i = cmdl.getArg(1), d=cmdl.getArg(2);
    // pid.SetTunings(p, i, d);
    break;
  }
  case 'A': {
    if (cmdl.getArgCount()<1) {
      ack = false;
      break;
    }
    target_angle = math::Angle::from_rad(cmdl.getArg(0));
    Serial.println(target_angle._radians);
    pm.set_target_angle(target_angle);
    break;
  }
  default:
  {
    ack = false;
    break;
  }
  }
  Serial.println(ack ? "!" : "?");
}


//-------------------------- MAIN PROG -----------------------------
void setup()
{
  Serial.begin(115200);
  CrcLib::Initialize();
  motor.begin();
  re.begin();
  pm.set_target_angle(target_angle);
}

void loop()
{
  cmdl.refresh();
  CrcLib::Update();
  pm.loop();

  // in = math::Angle::travel(re.getLast(), target_angle);
  // if (pid.Compute()) {
  //   motor.set_speed(out);
  // }

  // if (print_timer.is_time(millis()))
  // {
  //   Serial.println("a:"+ String(re.getLast()._radians) + " i:" + String(in) + " o:" + String(out));
  // }

  update_cmd();
}



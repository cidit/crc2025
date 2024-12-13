#include <Arduino.h>
#include <Decodeur.h>
#include <CrcLib.h>
#include <PID_RT.h>
#include "drives/precision_motor.hpp"
#include "drives/motor.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "Encoder.h"
#include "math/angles.hpp"

//-------------------------- DEFINES -----------------------------
#define TICKS_117 1425.1  //Bras
double TICKS_312 = 537.7; 
#define TICKS_1150 145.1 //Lanceur, swerve


//-------------------------- VARIABLES -----------------------------
double in, out, s=0;
auto target_angle = math::Angle::zero();


//-------------------------- OBJECTS -----------------------------
Decodeur cmdl(&Serial);

drives::Motor motor(CRC_PWM_1, false);
Encoder enco(CRC_ENCO_A, CRC_ENCO_B);
//sensors::GobuildaRotaryEnco re(enco, 537.7);
drives::PrecisionMotor pm(motor, enco, 1.0, 0.6, 0.0, 0.08, 0.08, 0.0002, 10, 312.0, TICKS_312);
//_timer print__timer(500); // ms
double _setpointS, _inputS, _outputS;
PID_RT _pidS;
int _last_enco;

int _delai = 10;
int _timer = millis();

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
    _setpointS = speed;
    Serial.print("Speed: ");
    Serial.println(speed);
    pm.set_target_speed(speed);
    break;
  }
  case 'K': {
    if (cmdl.getArgCount() < 3) {
      ack = false;
      break;
    }
    double p = cmdl.getArg(0), i = cmdl.getArg(1), d=cmdl.getArg(2);
    Serial.print("Tunings: " + String(p) + " " + String(i) + " " + String(d));
    pm.set_speed_pid(p, i, d);
    break;
  }
  case 'A': {
    if (cmdl.getArgCount()<1) {
      ack = false;
      break;
    }
    target_angle = math::Angle::from_rad(cmdl.getArg(0));
    Serial.print("Rads: ");
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
  //re.begin();
  //pm.set_target_angle(target_angle);
  //CrcLib::InitializePwmOutput(CRC_PWM_1); 

  // _last_enco = enco.read();
  // _pidS.setOutputRange(-128, 127);
  // _pidS.setInterval(_delai);
  // _setpointS = 312.0;
  // _pidS.setPoint(_setpointS);
  // _pidS.setK(0.08, 0.08, 0.0002);
  // _pidS.start();

  //CrcLib::SetPwmOutput(CRC_PWM_1, 127);
}

void loop()
{
  cmdl.refresh();
  update_cmd();

  CrcLib::Update();
  pm.loop();
  //Serial.println(enco.read());
  

  // double interval = millis() - _timer;
  // if(interval >= _delai){
  //   _timer = millis();

  //   int enco_out = enco.read();

  //   //Calculate current speed of motor
  //   auto distance_travelled = _last_enco - enco_out;
  //   auto current_rpm = (distance_travelled /TICKS_312 / (interval/1000))*60;

  //   _last_enco = enco_out;

  //   //constrain
  //   if(current_rpm > 312.0){
  //     current_rpm = 312.0;
  //   }else if(current_rpm < -312.0){
  //     current_rpm = -312.0;
  //   }
    
  //   //Compute using currrent speed
  //   Serial.print("current RPM: " + String(current_rpm));
  //   _inputS = current_rpm;
    
  // } 

  // if (_pidS.compute(_inputS)) {
  //   Serial.print("    set/targetRPM: " + String(_setpointS));
  //   Serial.print("    in/RPM: " + String(_inputS));
  //   _outputS = _pidS.getOutput();
  //   Serial.println("    out/Power: " + String(_outputS));

  //   CrcLib::SetPwmOutput(CRC_PWM_1, _outputS);
  // }
}



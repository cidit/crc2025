#include "Encoder.h"
#include <Decodeur.h>
#include <CrcLib.h>
#include <PID_RT.h>

const double TICKS_312 = 537.7; 

double setpoint, input, output;
int lastEnco;

int delai = 100;
const double maxRPM = 312.0;
int timer = millis();

Decodeur decodeur(&Serial);
Encoder enco(CRC_ENCO_A, CRC_ENCO_B);
PID_RT pid;

void setup() 
{
  Serial.begin(115200);
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_1, 1000, 2000); 

  lastEnco = enco.read();

  pid.setOutputRange(0, 128);
  pid.setInterval(delai);
  pid.setReverse(false);
  setpoint = 312;
  pid.setPoint(setpoint);
  pid.setK(1.0, 0.01, 0.0002);
  pid.start();
}

void loop() 
{
  CrcLib::Update();
  decodeur.refresh();
  update_cmd();

  double interval = millis() - timer;
  if(interval >= delai){
    timer = millis();

    int enco_out = enco.read();

    //Calculate current speed of motor
    auto diffEnco = enco_out - lastEnco;
    lastEnco = enco_out;

    auto currentRPM = (diffEnco /TICKS_312 / (interval/1000))*60;

    //Constrain
    if(currentRPM > maxRPM){
      currentRPM = maxRPM;
    }else if(currentRPM < -maxRPM){
      currentRPM = -maxRPM;
    }
    
    //Compute using currrent speed
    Serial.print("current RPM: " + String(currentRPM));
    input = fabs(currentRPM);
    
  } 

  if (pid.compute(input-setpoint)) {
    Serial.print("    set/targetRPM: " + String(setpoint));
    Serial.print("    in/RPM: " + String(input));
    output = pid.getOutput();
    Serial.println("    out/Power: " + String(output));

    CrcLib::SetPwmOutput(CRC_PWM_1, output);
  }
}

void update_cmd(){
  if (!decodeur.isAvailable())
  {
    return;
  }

  auto ack = true;
  switch (toupper(decodeur.getCommand()))
  {
    case 'S':
      if (decodeur.getArgCount() < 1)
      {
        ack = false;
        break;
      }
      auto speed = decodeur.getArg(0);
      Serial.println("setting speed to: " + String(speed));
      setpoint = speed;
      pid.setPoint(setpoint);
      //pm.set_target_speed(speed);
      break;

    case 'K': 
      if (decodeur.getArgCount() < 3) {
        ack = false;
        break;
      }
      double p = decodeur.getArg(0), i = decodeur.getArg(1), d=decodeur.getArg(2);
      pid.setK(p,i,d);
      //pm.set_speed_pid(p, i, d);
      break;

    case 'A': 
      if (decodeur.getArgCount()<1) {
        ack = false;
        break;
      }
      //target_angle = decodeur.getArg(0);
      //pm.set_target_angle(target_angle);
      break;

    default:
      ack = false;
      break;
  }
  Serial.println(ack ? "!" : "?");
}

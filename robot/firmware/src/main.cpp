#include <CrcLib.h>
#include <Encoder.h>
#include <PID_RT.h>
#include <Decodeur.h>

const double SETPOINT = 0;
int target_angle = 0;
float speed = 0, max_pulse = 0, min_pulse = 0;
bool checking_1 = true;
double joyAngle = 0;
double joySpeed = 1;



Decodeur decodeur(&Serial);
// PID_RT pid;

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
    if (newA > 180) newA -= 180;
    target_angle = newA;
    break;
  }
  case 'I': {
    // increment
    // TODO: temporary
    speed = decodeur.getArg(0);
    break;
  }
  case 'T' : {
    // TODO: 
    checking_1 = !checking_1;
    break;
  }
  default:
    ack = false;
  }
  Serial.println(ack? '!': '?');
}

double getCurrentAngle(){
  return pulseIn(CRC_PWM_12,HIGH)/4160.0*360;
}

void setMotorSpeed(double){

}

void setup()
{
  Serial.begin(115200);
  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  // Pinmode pour le PWM
  pinMode(CRC_PWM_12, INPUT);
}

void loop()
{
  decodeur.refresh();
  CrcLib::Update();
  apply_cmds();



  // if (checking_1) {
  //   CrcLib::SetPwmOutput(CRC_PWM_1, speed);
  //   CrcLib::SetPwmOutput(CRC_PWM_2, 0);
  // } else {
  //   CrcLib::SetPwmOutput(CRC_PWM_1, 0);
  //   CrcLib::SetPwmOutput(CRC_PWM_2, speed);
  // }

  // auto pulse = pulseIn(CRC_PWM_12, HIGH);
  // if (pulse > max_pulse)
  //   max_pulse = pulse;
  // if (pulse < min_pulse)
  //   min_pulse = pulse;

  // Serial.println(
  //     "pulse: " + String(pulse) +
  //     "\tmin pulse: " + String(min_pulse) +
  //     "\tmax pulse: " + String(max_pulse) +
  //     "\ttarg: " + String(target_angle));
}
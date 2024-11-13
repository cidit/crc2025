#include <CrcLib.h>
#include <Encoder.h>
#include <PID_RT.h>
#include <Decodeur.h>

const double SETPOINT = 0;

// Define the aggressive and conservative Tuning Parameters
const double aggKp = 4, aggKi = 0.2, aggKd = 1;
const double consKp = 1, consKi = 0.05, consKd = 0.25;



int target_angle = 0;

Decodeur decodeur(&Serial);
PID_RT pid;

void setup()
{
  Serial.begin(115200);
  // Initialisation des Moteurs
  CrcLib::Initialize();
  CrcLib::InitializePwmOutput(CRC_PWM_2);
  CrcLib::InitializePwmOutput(CRC_PWM_1);

  // Pinmode pour le PWM
  pinMode(CRC_PWM_12, INPUT);

  // Initialisation PID
  pid.setPoint(0);
  pid.setReverse(false);
  pid.setOutputRange(-128, 127);
  // pid.setK(0.5, 0.02, 0.008); // Proportionnal, Integral, Dirivative
  // pid.setK(aggKp, aggKi, aggKd); // Proportionnal, Integral, Dirivative
  // pid.setK(consKp, consKi, consKd); // Proportionnal, Integral, Dirivative
  // pid.setK(0.3, 0.01, 0); // Proportionnal, Integral, D
  pid.setK(0.9, 0, 0); // Proportionnal, Integral, Dirivative

}

void apply_cmds()
{
  if (decodeur.isAvailable())
  {
    switch (decodeur.getCommand())
    {
    case 'A':
    {
      auto newA = decodeur.getArg(0);
      if (newA > 180)
        newA -= 180;
      target_angle = newA;
      break;
    }
    default:
      break;
    }
  }
}

float read_curr_angle()
{
  // Calcul les angles
  auto angleEnco = pulseIn(CRC_PWM_12, HIGH);
  auto angleAct = angleEnco / 4160.0 * 360;
  if (angleAct > 180)
  {
    angleAct = angleAct - 180;
  }
  return angleAct;
}

void set_PWM(double angle)
{
  pid.start();
  if (pid.compute(angle))
  {
    CrcLib::SetPwmOutput(CRC_PWM_1, pid.getOutput());
    CrcLib::SetPwmOutput(CRC_PWM_2, -pid.getOutput());
  }
  else
  {
    pid.stop();
  }
}

// static uint32_t counter = 0;

void loop()
{
  // counter++;
  // Serial.println(counter);
  decodeur.refresh();
  CrcLib::Update();
  apply_cmds();

  auto current_angle = read_curr_angle();

  auto norAngle = SETPOINT - current_angle + target_angle;
  auto oppAngle = 180 + norAngle;

  if (abs(norAngle) > abs(oppAngle))
  {
    // counter clockwise
    set_PWM(oppAngle);
  }
  else
  {
    // clockwise
    set_PWM(norAngle);
  }
  Serial.println("curr: "+ String(current_angle)+ "\ttarg: " + String(target_angle));
}
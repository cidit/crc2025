#include <CrcLib.h>
#include <Encoder.h>
#include <PID_RT.h>
#include <Decodeur.h>

const int MAX_MOTEUR_POWER = 127;
const int MAX_ANGLE_ERROR = 5; //Allow 5 deg dif from target angle
const double SETPOINT = 0;

struct Vector2D{
  float x = 0;
  float y = 0;
};

float speed = 0, max_pulse = 0, min_pulse = 0;
bool checking_1 = true;
double joyAngle = 0;
double joySpeed = 1;

int vecPowX = 1;
int vecPowY = 1;

Vector2D vecPower; //x=translationPower   y=rotationPower

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

/**
 * Returns the current wheel angle between 0 and 180 deg
 */
double getCurrentAngle(){
  //Get encoder value, make sure the value is between 0 and 4160 (Observation de Félix: L'encodeur retour des fois 8000)
  double enco = constrain((CRC_PWM_12, HIGH), 0.0, 4160.0);

  //Calculate angle
  double angleAct = enco/4160.0*360;
  
  //set angle between 0 and 180
  if (angleAct > 180)
  {
    angleAct = angleAct - 180;
  }
  return angleAct;
}

/**
 * Determine and set the power of the motors
 */
void setMotorPowers(Vector2D powerVector){
  //TODO Verify this
  double powerA = constrain(powerVector.x + powerVector.y, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);
  double powerB = constrain(powerVector.x - powerVector.y, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);

  CrcLib::SetPwmOutput(CRC_PWM_1, powerA);
  CrcLib::SetPwmOutput(CRC_PWM_2, powerB);
}

/**
 * Get and apply the translation and rotation components
 * based on the move joystick(left) angle and scale
 */
void moveModule(double targetAngle, double targetSpeedFactor){

  vecPower.x = 0;

}

/**
 * Get the X component of the power vector, so the translation power
 * the return value must be between 0 and 1
 */
double getTransComponent(double targetSpeed){
  //TODO
}

/**
 * Get the Y component of the power vector, so the rotation power
 * the return value must be between 0 and 1
 */
double getPivotComponent(double targetAngle, double currentAngle){
  //Calculate the difference between curent and target, corresponds to rotation angle
  auto travelAngle = SETPOINT - currentAngle + targetAngle;

  //Change for shortest if to big
  if(travelAngle < -90){
    travelAngle = 180 + travelAngle;
  }

  //Determine the power ratio of the motor, 1=fullPower 0=notMoving
  if(travelAngle < MAX_ANGLE_ERROR){
    return 0; //We are close enough
  }
  else
  {
    return 1; //TODO
  }
  
}

//---------------------------------------------------------------------------
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

  moveModule(joyAngle, joySpeed);

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
/*
//   Main program for robot control


//   SwerveAH : PWM_4
//   SwerveAB : PWM_3
//   SwerveBH : PWM_7
//   SwerveBB : PWM_1
//   PincePoignet : PWM_2
//   BrasA : PWM_6
//   BrasB : PWM_5

//   Servo1: PWM_9
//   Servo2: PWM_10
//   Servo3: PWM_11

//   Pour encodeur: Blanc=A, Jaune=B
//*/

#include <CrcLib.h>
#include <Encoder.h>
#include <drives/swerve_module.hpp>
#include <controller.hpp>
#include "util/looped.hpp"
#include "math/vectors.hpp"
#include <Servo.h>
#include <Decodeur.h>
#include <PID_RT.h>
using math::cartesian::Vec2D;

#define SP(things) Serial.print(things)

//----- Pin Defines -----
#define ABS_ENC_A CRC_DIG_1
#define ABS_ENC_B CRC_DIG_2
#define SW_BH CRC_PWM_7

//----- OBJ -----
drives::Motor motorBH(SW_BH);
Encoder encoBH(CRC_DIG_5,CRC_I2C_SCL);
drives::PrecisionMotor pmBH(motorBH, encoBH, 20, 1150.0, drives::PrecisionMotor::TICKS_1150);

// //----- Variables -----
Controller ctrl;
Decodeur cmd(&Serial);
Timer print_timer(100);

double targetPow = 0;
double Kp;
double Ki;
double Kd;
bool read_mode = false;


const int MAX_MOTOR_POWER = 127;

//----- Main Program -----
void setup()
{
    Serial.begin(115200);

    CrcLib::Initialize();
    pmBH.begin();

    Serial.println("Setup Done");
}

void loop()
{
    auto now = millis();
    CrcLib::Update();
    // ctrl.update();
    cmd.refresh();
    pmBH.loop();

    switch (toupper(cmd.getCommand()))
    {
    case 'T':
    {
        const auto max_rpm = 1150.0;
        const auto pow = cmd.getArg(0) / max_rpm;
        targetPow = constrain(pow, -128, 127);
        pmBH.set_target_power(targetPow);
        Serial.println("Target Power: " + String(targetPow));
        break;
    }
    case 'K':
    {
        Kp = cmd.getArg(0);
        Ki = cmd.getArg(1);
        Kd = cmd.getArg(2);
        pmBH.set_speed_pid(Kp, Ki, Kd);
        Serial.println("Kp: " + String(Kp, 5) + " Ki: " + String(Ki, 5) + " Kd: " + String(Kd, 5));
        break;
    }
    case 'P':
    {
        Kp = cmd.getArg(0);
        Ki = pmBH._pidS.getKi();
        Kd = pmBH._pidS.getKd();
        pmBH.set_speed_pid(Kp, Ki, Kd);
        Serial.println("Kp: " + String(Kp, 5) + " Ki: " + String(Ki, 5) + " Kd: " + String(Kd, 5));
        break;
    }
    case 'I':
    {
        Kp = pmBH._pidS.getKp();
        Ki = cmd.getArg(0);
        Kd = pmBH._pidS.getKd();
        pmBH.set_speed_pid(Kp, Ki, Kd);
        Serial.println("Kp: " + String(Kp, 5) + " Ki: " + String(Ki, 5) + " Kd: " + String(Kd, 5));
        break;
    }
    case 'D':
    {
        Kp = pmBH._pidS.getKp();
        Ki = pmBH._pidS.getKi();
        Kd = cmd.getArg(0);
        pmBH.set_speed_pid(Kp, Ki, Kd);
        Serial.println("Kp: " + String(Kp, 5) + " Ki: " + String(Ki, 5) + " Kd: " + String(Kd, 5));
        break;
    }
    case 'M':
    {
        // pmBH._pidS.setK(0, 0, 0);
        pmBH._current_motor_out = 0;
        break;
    }
    case 'R':
    {
        read_mode = !read_mode;
        break;
    }
    }

    if (read_mode && print_timer.is_time(now)) {
        double speed = pmBH.get_input();
        Serial.println("Current Speed: " + String(speed));
    }
}

// //----- SWERVE A (gauche) -----
// drives::Motor motorAH(CRC_PWM_4);
// // Encoder encoAH(CRC_ENCO_A, CRC_DIG_2);
// // drives::PrecisionMotor pmAH(motorAH, encoAH, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
// drives::Motor motorAB(CRC_PWM_3);
// // Encoder encoAB(CRC_I2C_SDA, CRC_DIG_4);
// // drives::PrecisionMotor pmAB(motorAB, encoAB, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
// // SwerveModule swerveA(pmAH, pmAB, CRC_PWM_12);
// PID_RT pidSwerveA;

// // //----- SWERVE B (droite) -----
// drives::Motor motorBH(CRC_PWM_7);
// // Encoder encoBH(CRC_I2C_SCL, CRC_DIG_5);
// // drives::PrecisionMotor pmBH(motorBH, encoBH, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
// drives::Motor motorBB(CRC_PWM_1);
// Encoder encoBB(CRC_ENCO_B, CRC_DIG_3);
// drives::PrecisionMotor pmBB(motorBB, encoBB, 10, 1150.0, drives::PrecisionMotor::TICKS_1150);
// // SwerveModule swerveB(pmBH, pmBB, CRC_DIG_1);
// // PID_RT pidSwerveB;

// // //----- Main Program ----------------------------------------------------------------------
// // void setup(){
// //   Serial.begin(115200);

// //   // Initialisation des Moteurs
// //   CrcLib::Initialize();
// //   //CrcLib::InitializePwmOutput(CRC_PWM_2);
// //   //CrcLib::InitializePwmOutput(CRC_PWM_1);

// //   swerveA.begin();
// //   swerveB.begin();

// //   Serial.println("Setup Done");
// // }

// // void loop(){
// //   CrcLib::Update();

// //   ctrl.update();
// //   cmd.refresh();

// //   // if(cmd.isAvailable()){
// //   //   auto commande = cmd.getCommand();
// //   //   switch(commande){
// //   //     case 'P':
// //   //         pmO.set_target_power(cmd.getArg(0));
// //   //         break;
// //   //   }
// //   // }

// //   //Serial.println(ctrl.get_left_joy().angleRad, ctrl.get_left_joy().norm);
// //   swerveA.set_target(ctrl.get_left_joy().angleRad , ctrl.get_left_joy().norm);
// //   swerveB.set_target(ctrl.get_left_joy().angleRad , ctrl.get_left_joy().norm);

// //   swerveA.loop();
// //   swerveB.loop();

// //   Serial.println();

// #define SP(things) Serial.print(things)

// enum class Direct
// {
//     CLOCKWISE,
//     COUNTERCLOCKWISE,
// };
// Direct dir;

// const int MAX_MOTEUR_POWER = 60;
// double powX = 0;
// double powY = 0;
// double angleOff = 0;
// double targetAngle = 0;

// double get_abs_enc_angle(uint8_t pin){
//   const auto MAX_PULSE_LEN = 4160.0;
//   double pulse = pulseIn(pin, HIGH, 8320); // TODO: if there are issues later, check the pulseIn
//   double angle = pulse / MAX_PULSE_LEN * (2 * M_PI);
//   return angle;
// }

// double get_diff_angle(double currentAngle){
//   //Calculate travel angle, will be negative
//   double diff = targetAngle - currentAngle;

//   if(diff >= 0){
//       dir = diff < M_PI ?
//                       diff > M_PI/2 ? Direct::CLOCKWISE:Direct::COUNTERCLOCKWISE
//                       :diff > 3*M_PI/2 ? Direct::CLOCKWISE:Direct::COUNTERCLOCKWISE;
//   }
//   else if(diff < 0){
//       dir = diff > -M_PI ?
//                       diff > -M_PI/2 ? Direct::CLOCKWISE:Direct::COUNTERCLOCKWISE
//                       :diff > -3*M_PI/2 ? Direct::CLOCKWISE:Direct::COUNTERCLOCKWISE;
//   }

//   // //Keep value in a cercle
//   // while(diff > 2*M_PI){
//   //   diff = diff - 2*M_PI;
//   // }
//   // while(diff < -2*M_PI){
//   //   diff = diff + 2*M_PI;
//   // }

//   return diff;
// }

// double get_translation_component(double diff, double speedFactor){
//   double component = cos(diff)*speedFactor;
//   return component;
// }

// double get_angular_component(double diff, double speedFactor){
//   double component = sin(diff)*speedFactor;
//   return component;
// }

// double get_PID_angle(double diff){
//   if(diff > 0){
//       //Keep angle between 0 and M_PI
//       if(diff > M_PI){
//           diff -= M_PI;
//       }

//       //Take shortest angle
//       if(diff > M_PI_2){
//           diff = M_PI - diff;
//       }
//       else{
//           diff = -fabs(diff);
//       }

//   }
//   else if(diff < 0){
//       //Keep angle between 0 and M_PI
//       if(diff < -M_PI){
//           diff += M_PI;
//       }

//       //Take shortest angle
//       if(diff < -M_PI_2){
//           diff = M_PI + diff;
//       }
//       else{
//           diff = -fabs(diff);
//       }
//   }

//   return diff;
// }

// //----------------------------------------------------
// // Main Program
// void setup()
// {
//   Serial.begin(115200);
//   CrcLib::Initialize();
//   motorAB.begin();
//   motorAH.begin();
//   motorBB.begin();
//   motorBH.begin();

//   pidSwerveA.setPoint(0);
//   pidSwerveA.setOutputRange(-1.0, 1.0);
//   pidSwerveA.setK(0.4, 0, 0);
//   pidSwerveA.setInterval(10);
//   pidSwerveA.setPropOnError();

//   pidSwerveA.start();
//   angleOff = get_abs_enc_angle(ABS_ENC_A);
// }

// void loop()
// {
//   ctrl.update();
//   cmd.refresh();

//   static uint32_t last_now = 0;
//   static uint32_t iteration = 0;

//   auto now = millis();

//   SP(String(iteration++));
//   SP("   ");
//   SP(now);
//   SP("   ");
//   SP(String(now - last_now));

//   last_now = now;

//   CrcLib::Update();

//   switch(cmd.getCommand()){
//     case 'T':
//       targetAngle = cmd.getArg(0);
//       break;
//     case 'K':
//       pidSwerveA.setK(cmd.getArg(0), cmd.getArg(1), cmd.getArg(2));
//       break;
//   }

//   // if (!CrcLib::IsCommValid())
//   // {
//   //   SP("\tCOMINV");
//   //   motorAB.set_power(0);
//   //   motorAH.set_power(0);
//   //   motorBB.set_power(0);
//   //   motorBH.set_power(0);
//   // }
//   // else
//   // {
//   //   motorAB.set_power(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X)/2);
//   //   motorAH.set_power(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)/2);
//   //   motorBB.set_power(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X)/2);
//   //   motorBH.set_power(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y)/2);
//   // }

//   // Serial.print("   j1x:");
//   // Serial.print(ctrl.joyLeft.x);
//   // Serial.print("   j1y:");
//   // Serial.print(ctrl.joyLeft.y);
//   // Serial.print("   j2x:");
//   // Serial.print(ctrl.joyRight.x);
//   // Serial.print("   j2y:");
//   // Serial.print(ctrl.joyRight.x);
//   // SP("\teA:");
//   // SP(String(get_abs_enc_angle(ABS_ENC_A)));
//   // SP("\teB:");
//   // SP(String(get_abs_enc_angle(ABS_ENC_B)));

//   //SWERVE
//   double currentAngle = get_abs_enc_angle(ABS_ENC_A);
//   double diffAngle = get_diff_angle(currentAngle);
//   SP("\tcA:");
//   SP(String(currentAngle));
//   SP("\ttA:");
//   SP(String(targetAngle));
//   SP("\tdA:");
//   SP(String(diffAngle));
//   double transPow = ctrl.joyLeft.norm;
//   SP("\tnorm:");
//   SP(String(transPow));

//   if(transPow != 0 && pidSwerveA.compute(get_PID_angle(diffAngle))){
//     powX = get_translation_component(fabs(diffAngle), fabs(transPow));
//     powY = fabs(pidSwerveA.getOutput());;
//   }
//   // else if(transPow == 0){
//   //   powX = 0;
//   //   powY = 0;
//   // }

//   //Apply the good sign
//   switch(dir){
//     case Direct::CLOCKWISE:
//         powY = fabs(powY);
//         break;
//     case Direct::COUNTERCLOCKWISE:
//         powY = -fabs(powY);
//         break;
//   }

//   if(ctrl.joyLeft.angleRad <= M_PI){
//     //Forward
//     if(currentAngle <= M_PI){
//       powX = fabs(powX);
//     }else{
//       powX = -fabs(powX);
//     }
//   }else{
//     //Backward
//     if(currentAngle <= M_PI){
//       powX = -fabs(powX);
//     }else{
//       powX = fabs(powX);
//     }
//   }
//   SP("\tpX:");
//   SP(String(powX));
//   SP("\tpY:");
//   SP(String(powY));
//   double powA = powY + powX;
//   double powB = powY - powX;
//   // SP("\tpA:");
//   // SP(String(powA));
//   // SP("\tpB:");
//   // SP(String(powB));

//   powA = constrain(powA*MAX_MOTEUR_POWER, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);
//   powB = constrain(powB*MAX_MOTEUR_POWER, -MAX_MOTEUR_POWER, MAX_MOTEUR_POWER);
//   SP("\tpA:");
//   SP(String(powA));
//   SP("\tpB:");
//   SP(String(powB));

//   motorAB.set_power(powA);
//   motorAH.set_power(powB);

//   Serial.println();
// }

// /*#################################################################################################################################
//   Optimized program for robot control
//   - ChatGPT
// */
// #include <CrcLib.h>
// #include <Encoder.h>
// #include <drives/swerve_module.hpp>
// #include <controller.hpp>
// #include "util/looped.hpp"
// #include "math/vectors.hpp"
// #include <Servo.h>
// #include <Decodeur.h>
// #include <PID_RT.h>
// using math::cartesian::Vec2D;

// //----- Pin Defines -----
// #define ABS_ENC_A CRC_DIG_12
// #define ABS_ENC_B CRC_DIG_1

// //----- Variables -----
// Controller ctrl;
// Decodeur cmd(&Serial);

// drives::Motor motorAH(CRC_PWM_4);
// drives::Motor motorAB(CRC_PWM_3);
// PID_RT pidSwerveA;

// enum class Direct { CLOCKWISE, COUNTERCLOCKWISE };
// Direct dir;

// const int MAX_MOTOR_POWER = 60;
// double powX = 0, powY = 0;
// double targetAngle = 0;

// // Autotuning variables
// bool autotuning = false;
// double ku = 0;  // Ultimate gain
// int tu = 0;     // Ultimate period
// uint32_t autotune_start_time = 0;
// double last_output = 0;
// double max_output = -1e6;
// double min_output = 1e6;
// uint32_t last_peak_time = 0;
// bool rising = true;

// void start_autotuning() {
//     autotuning = true;
//     autotune_start_time = millis();
//     ku = tu = 0;
//     max_output = -1e6;
//     min_output = 1e6;
//     last_peak_time = 0;
//     rising = true;
//     Serial.println("Start of autotune");
// }

// void stop_autotuning() {
//     autotuning = false;
//     if (tu > 0) {
//         pidSwerveA.setK(0.6 * ku, 1.2 * ku / tu, 0.075 * ku * tu);
//     }
//     Serial.println("End of autotune");
//     Serial.print("Params: "); Serial.print(pidSwerveA.getKp()); Serial.print(", "); Serial.print(pidSwerveA.getKi()); Serial.print(", "); Serial.print(pidSwerveA.getKd());
// }

// double get_abs_enc_angle(uint8_t pin) {
//     return pulseIn(pin, HIGH, 8320) / 4160.0 * (2 * M_PI);
// }

// double normalize_angle(double angle) {
//     return fmod(angle + M_PI, 2 * M_PI) - M_PI;
// }

// double get_PID_angle(double diff) {
//     return (diff > M_PI || diff < -M_PI) ? M_PI - fabs(diff) : -fabs(diff);
// }

// //----------------------------------------------------
// void setup() {
//     Serial.begin(115200);
//     CrcLib::Initialize();
//     motorAB.begin();
//     motorAH.begin();

//     pidSwerveA.setPoint(0);
//     pidSwerveA.setOutputRange(-1.0, 1.0);
//     pidSwerveA.setK(0.4, 0, 0);
//     pidSwerveA.setInterval(10);
//     pidSwerveA.start();
// }

// void loop() {
//     ctrl.update();
//     cmd.refresh();
//     CrcLib::Update();

//     static uint32_t last_now = 0;
//     uint32_t now = millis();
//     last_now = now;

//     switch (cmd.getCommand()) {
//         case 'T': targetAngle = cmd.getArg(0); break;
//         case 'K': pidSwerveA.setK(cmd.getArg(0), cmd.getArg(1), cmd.getArg(2)); break;
//         case 'A': start_autotuning(); break;
//     }

//     double currentAngle = get_abs_enc_angle(ABS_ENC_A);
//     double diffAngle = normalize_angle(targetAngle - currentAngle);

//     if (autotuning) {
//         double output = sin(diffAngle) * 100;
//         motorAB.set_power(output);
//         motorAH.set_power(-output);

//         if (output > max_output) {
//             max_output = output;
//             if (!rising) {
//                 rising = true;
//                 if (last_peak_time > 0) {
//                     tu = now - last_peak_time;
//                 }
//                 last_peak_time = now;
//             }
//         } else if (output < min_output) {
//             min_output = output;
//             if (rising) {
//                 rising = false;
//             }
//         }

//         ku = (max_output - min_output) / 2.0;

//         if (millis() - autotune_start_time > 5000) stop_autotuning();
//     } else {
//         if (ctrl.joyLeft.norm != 0 && pidSwerveA.compute(get_PID_angle(diffAngle))) {
//             powX = cos(diffAngle) * ctrl.joyLeft.norm;
//             powY = fabs(pidSwerveA.getOutput());
//         } else {
//             powX = powY = 0;
//         }

//         powY = (dir == Direct::COUNTERCLOCKWISE) ? -fabs(powY) : fabs(powY);
//         powX = (currentAngle <= M_PI) ? fabs(powX) : -fabs(powX);

//         double powA = constrain((powY + powX) * MAX_MOTOR_POWER, -MAX_MOTOR_POWER, MAX_MOTOR_POWER);
//         double powB = constrain((powY - powX) * MAX_MOTOR_POWER, -MAX_MOTOR_POWER, MAX_MOTOR_POWER);

//         motorAB.set_power(powA);
//         motorAH.set_power(powB);
//     }

//     //Serial.println();
// }

/*
 * Programme principal du robot 
 * Contient la logique de fonctionnement.
 *
*/

//----- Includes -----
#include <CrcLib.h>
#include "communication/controller.hpp"
#include <drives/precision_motor.hpp>
#include <Encoder.h>
#include <sensors/gobuilda_rotary_enc.hpp>
#include "sensors/paul_stroffregen_enco_sensor_wrapper.hpp"
//--------------------

//----- Defines ------
//--------------------

//----- Objects ------
Controller ctrl;
Timer polling_timer(ONE_SECOND / 40);

//Moteur pour le bras
    //Moteur Gauche
Motor motorLB(CRC_PWM_7);
Encoder encoLB(CRC_I2C_SCL, CRC_DIG_5);
EncoderWrapper enco_wrappedLB(encoLB, polling_timer);
GobuildaRotaryEncoder roencoLB(enco_wrappedLB, 145.1 * 2.5, polling_timer, true);
PrecisionMotor pmotorLB(motorLB, roencoLB, 400.);
    //Moteur Droit
Motor motorRB(CRC_PWM_7);
Encoder encoRB(CRC_I2C_SCL, CRC_DIG_5);
EncoderWrapper enco_wrappedRB(encoRB, polling_timer);
GobuildaRotaryEncoder roencoRB(enco_wrappedRB, 145.1 * 2.5, polling_timer, true);
PrecisionMotor pmotorRB(motorRB, roencoRB, 400.);
    //Moteur Poignet
Motor motorPB(CRC_PWM_7);
Encoder encoPB(CRC_I2C_SCL, CRC_DIG_5);
EncoderWrapper enco_wrappedPB(encoPB, polling_timer);
GobuildaRotaryEncoder roencoPB(enco_wrappedPB, 145.1 * 2.5, polling_timer, true);
PrecisionMotor pmotorPB(motorPB, roencoPB, 400.);
    //Moteur Servo
    //TODO

//Moteur pour swerve A
    //Moteur A
    //TODO
    //Moteur B
    //TODO

//Moteur pour swerve B
    //Moteur A
    //TODO
    //Moteur B
    //TODO

//--------------------

//----- Variables ----
//--------------------


//----- Main Prog ----
void setup(){
    Serial.begin(115200);
    CrcLib::Initialize();
}

void loop(){
    //Updates every object
    CrcLib::Update();
    ctrl.update();
    
    //Gère les différents objects selon les inputs
    lanceur();
    swerve();
    bras();
}
//--------------------

/// @brief Logique du lanceur
void lanceur(){

}

/// @brief Logique des swerves
void swerve(){

}

/// @brief Logique du bras
void bras(){

}
//--------------------
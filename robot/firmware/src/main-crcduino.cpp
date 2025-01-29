/*
 * Programme principal du robot 
 * Contient la logique de fonctionnement.
 *
*/

//----- Includes -----
#include <Arduino.h>
#include <CrcLib.h>
#include "communication/controller.hpp"
#include "communication/enco_peripherals.hpp"
#include <drives/precision_motor.hpp>
#include <drives/swerve_module.hpp>
#include <Encoder.h>
#include <sensors/gobuilda_rotary_enc.hpp>
#include <sensors/lin_enco_spoof.hpp>
#include "sensors/paul_stroffregen_enco_sensor_wrapper.hpp"
#include <Servo.h>
#include <SPI.h>
#include "util/timer.hpp"
//--------------------

//----- Defines ------
#define TICKS_RATIO_BRAS 1425.1 *2
#define TICKS_RATIO_SWERVE 145.1 * 2.5
#define TICKS_RATIO_LANCE 147

#define MAX_RPM_BRAS 117
#define MAX_RPM_SWERVE 400
#define MAX_RPM_LANCE 1150

#define NB_PM 8 
    //Index in array (Selon les branchements sur le ALDuino)
    #define I_LAS 2
    #define I_LBS 3
    #define I_RAS 1
    #define I_RBS 0
    #define I_LB  5
    #define I_RB  4
    #define I_PB  6
    #define I_L   7

//--------------------

//----- Objects ------
Controller ctrl; //La manette
Timer polling_timer(ONE_SECOND / 1000); //Doit au moins être de 1ms
int32_t df[NB_PM]; //Le dataframe qui reçoit les valeurs du ALDuino

Motor motors[NB_PM] = {
    {CRC_PWM_7}, //Swerve Right B
    {CRC_PWM_1}, //Swerve Right A
    {CRC_PWM_3}, //Swerve Left A
    {CRC_PWM_4}, //Swerve Left B
    {CRC_PWM_5}, //Bras Right
    {CRC_PWM_6}, //Bras Left
    {CRC_PWM_2}, //Poignet
    {CRC_PWM_8}, //À Déterminé - Lanceur
};

LinEncSpoof spoofs[NB_PM] = {
    {df[0], polling_timer}, //Swerve Right B 
    {df[1], polling_timer}, //Swerve Right A
    {df[2], polling_timer}, //Swerve Left A
    {df[3], polling_timer}, //Swerve Left B
    {df[4], polling_timer}, //Bras Right
    {df[5], polling_timer}, //Bras Left
    {df[6], polling_timer}, //Poignet
    {df[7], polling_timer}, //Lanceur
};

GobuildaRotaryEncoder goencs[NB_PM] = {
    {spoofs[0], TICKS_RATIO_SWERVE, polling_timer}, //Swerve Right B 
    {spoofs[1], TICKS_RATIO_SWERVE, polling_timer}, //Swerve Right A
    {spoofs[2], TICKS_RATIO_SWERVE, polling_timer, true}, //Swerve Left A
    {spoofs[3], TICKS_RATIO_SWERVE, polling_timer}, //Swerve Left B
    {spoofs[4], TICKS_RATIO_BRAS, polling_timer}, //Bras Right
    {spoofs[5], TICKS_RATIO_BRAS, polling_timer}, //Bras Left
    {spoofs[6], TICKS_RATIO_BRAS, polling_timer}, //Poignet
    {spoofs[7], TICKS_RATIO_LANCE, polling_timer}, //Lanceur
};

PrecisionMotor pmotors[NB_PM] = {
    {motors[0], goencs[0], MAX_RPM_SWERVE}, //Swerve Right B 
    {motors[1], goencs[1], MAX_RPM_SWERVE}, //Swerve Right A
    {motors[2], goencs[2], MAX_RPM_SWERVE}, //Swerve Left A
    {motors[3], goencs[3], MAX_RPM_SWERVE}, //Swerve Left B
    {motors[4], goencs[4], MAX_RPM_BRAS}, //Bras Right
    {motors[5], goencs[5], MAX_RPM_BRAS}, //Bras Left
    {motors[6], goencs[6], MAX_RPM_BRAS}, //Poignet
    {motors[7], goencs[7], MAX_RPM_LANCE}, //Lanceur
};

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco_right(CRC_DIG_1, MAX_PULSE_LEN, 0.0, polling_timer); // TODO: get right pin
PwmRotaryEncoder pwm_enco_left(CRC_DIG_2, MAX_PULSE_LEN, 0.0, polling_timer);

SwerveModule swerve_right(pmotors[I_RAS], pmotors[I_RBS], pwm_enco_right);
SwerveModule swerve_left(pmotors[I_LAS], pmotors[I_LBS], pwm_enco_left);

//--------------------

//----- Variables ----
//--------------------


//----- Main Prog ----
void setup(){
    Serial.begin(115200);
    CrcLib::Initialize();

    //Init Bras
    pmotors[I_LB]._pid_angle.setInterval(polling_timer._delay);
    pmotors[I_RB]._pid_angle.setInterval(polling_timer._delay);
    pmotors[I_PB]._pid_angle.setInterval(polling_timer._delay);
    pmotors[I_LB].begin();
    pmotors[I_RB].begin();
    pmotors[I_PB].begin();
    pmotors[I_LB].enable(true);
    pmotors[I_RB].enable(true);
    pmotors[I_PB].enable(true);
}

void loop(){
    //Updates every object
        CrcLib::Update();
        ctrl.update();

        //Bras
        pmotors[I_LB].update();
        pmotors[I_RB].update();
        pmotors[I_PB].update();
    
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
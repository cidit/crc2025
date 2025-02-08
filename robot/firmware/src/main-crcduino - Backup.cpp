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

#define VIT_BRAS_MS M_PI/2000.0 *0.1
#define LOW_STOP_BRAS 0.0
#define HIGH_STOP_BRAS 3.14

#define SPRINT(things) Serial.print(things)

enum RobotState{
    HORIZONTAL_FEED,
    VERTICAL_FEED,
    GROUND_FEED,
    HORIZONTAL_POLL,
    VERTICAL_POLL,
    INVERTED_POLL,
    LAUNCHING,
    MOVING
};

//--------------------

//----- Objects ------
RobotState robot_state = MOVING;
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
unsigned long last_time = millis();
float angle_bras = 0;
//--------------------


//----- Main Prog ----

void update_df()
{
    // TODO: for some reason, the values seem to be multiplied by 256
    retrieve_df(df);
    // here, we sorta patch the multiplied by 256 problem
    // for (int i = 0; i < ENCO_NUM; i++)
    // {
    //     df[i] /= 256;
    // }
}

String padLeft(String inString, uint16_t newLen, char c)
{ // Pad a numeric string with spaces for output
    while (inString.length() < newLen)
    {
        inString = String(c) + inString;
    };
    return inString;
}


String padRight(String inString, uint16_t newLen, char c)
{ // Pad a numeric string with spaces for output
    while (inString.length() < newLen)
    {
        inString = inString + String(c);
    };
    return inString;
}

void setup(){
    Serial.begin(115200);
    //CrcLib::Initialize();

    master_enco_spi_init();
    SPI.begin();

    //Init Bras
    pmotors[I_LAS]._pid_speed.setInterval(polling_timer._delay);
    pmotors[I_LBS]._pid_speed.setInterval(polling_timer._delay);
    pmotors[I_RAS]._pid_speed.setInterval(polling_timer._delay);
    pmotors[I_RBS]._pid_speed.setInterval(polling_timer._delay);

    pmotors[I_LB ]._pid_angle.setInterval(polling_timer._delay);
    pmotors[I_LB ]._pid_angle.setK(1, 0, 0);
    pmotors[I_RB ]._pid_angle.setInterval(polling_timer._delay);
    pmotors[I_RB ]._pid_angle.setK(1, 0, 0);

    pmotors[I_PB ]._pid_angle.setInterval(polling_timer._delay);
    pmotors[I_L  ]._pid_speed.setInterval(polling_timer._delay);
    
    // for (auto &pmotor : pmotors)
    // {
    //     pmotor.begin();
    //     pmotor.enable(true);
    // }

    Serial.println("End of setup");
}

void loop(){
    polling_timer.update(millis());

    if (polling_timer.is_time())
    {
        retrieve_df(df);
    }

    //TESTING GOBILDA ENCS
    if (polling_timer.is_time())
    {
        for (auto i = 0; i < ENCO_NUM; i++)
        {
            goencs[i].update();
            SPRINT("| e");
            SPRINT(i);
            SPRINT(" ");
            SPRINT(String(goencs[i].getLast().rads, 1));
            SPRINT(" ");
            SPRINT(String(goencs[i].getLast().rpm, 1));
            SPRINT(" ");
            SPRINT(padLeft(String(goencs[i]._ie.getLast()), 3, '0'));
        }
        hexdump_df(df);
        Serial.println("|");
    }


    // //Updates every object
    // unsigned long now = millis();
    // unsigned long delta = now - last_time;
    // last_time = now;
    //CrcLib::Update();
    // ctrl.update();
    // polling_timer.update(now);

    // // pmotors[I_LB].update();
    // // pmotors[I_RB].update();

    // if (polling_timer.is_time())
    // {
    //     retrieve_df(df);
    //     goencs[4].update();
    //     goencs[5].update();
    //     Serial.println("df"+ String(df[4])+"   Enco4: "+ String(spoofs[4].getLast())+ "      Go4: "+String(goencs[4].getLast().rads));
    //     Serial.println("df"+ String(df[5])+"   Enco5: "+ String(spoofs[5].getLast())+ "      Go5: "+String(goencs[5].getLast().rads));
    //     Serial.println();
    // }


    

    // // for (auto &pmotor : pmotors)
    // // {
    // //     pmotor.update();
    // // }
    
    // //CrcLib::SetPwmOutput(CRC_PWM_2, 128);
    // //rotation base bras
    // if(ctrl.gachettes.Left){
    //     //IN
    //     angle_bras -= ctrl.gachettes.Left * delta * VIT_BRAS_MS;
    //     //Serial.println(ctrl.gachettes.Right * delta * (M_PI/1000.0));
    //     //Serial.println(angle_bras);
    //     angle_bras = constrain(angle_bras, LOW_STOP_BRAS, HIGH_STOP_BRAS);
    //     //pmotors[I_LB].set_target_angle(-angle_bras);
    //     pmotors[I_RB].set_target_angle(angle_bras);
    // }
    // else if(ctrl.gachettes.Right){
    //     //OUT
    //     angle_bras += ctrl.gachettes.Right * delta * VIT_BRAS_MS;
    //     //Serial.println(ctrl.gachettes.Right * delta * (M_PI/1000.0));
    //     //Serial.println(angle_bras);
    //     angle_bras = constrain(angle_bras, LOW_STOP_BRAS, HIGH_STOP_BRAS);
    //     //pmotors[I_LB].set_target_angle(-angle_bras);
    //     pmotors[I_RB].set_target_angle(angle_bras);
    // }
    //Serial.println(angle_bras);
    // //Gestion des Inputs (Mise en position)
    // if(ctrl.buttons.A){ //Lanceur    
    //     // Positionne bras
    //     // Tourner poignet
    //     // Active moteur
    //     // Drop pièce
    //     // Pousser pièce

    // }
    // else if(ctrl.buttons.B){ //Poteau Vertical

    //     // Monte bras
    //     // Tourner poignet  

    // }
    // else if(ctrl.buttons.Y){ //Poteau Inversé

    //     // Placer bras
    //     // Tourner poignet   

    // }
    // else if(ctrl.buttons.X){ //Poteau Horizontal     

    //     // Placer bras
    //     // Tourner poignet   

    // }
    // // else if(ctrl.buttons.Up){
        
    // // }
    // // else if(ctrl.buttons.Down){

    // // }
    // // else if(ctrl.buttons.Left){

    // // }
    // // else if(ctrl.buttons.Right){

    // // }
    // else if(ctrl.buttons.LBumper){ //Distributeur Vertical      

    //     // Positionner bras     
    //     // Orienter Poignet 
    // }
    // else if(ctrl.buttons.RBumper){ //Distributeur Horizontal   

    //     // Positionner bras     
    //     // Orienter Poignet
    // }
    // else if(ctrl.buttons.RJoy){ //Distributeur Sol

    //     // Descendre bras
    //     // Bien orienter poignet
    // }


    // //Trigger Gachette
    // if(ctrl.gachettes.Left >= 0){ //End action
    //     switch (robot_state)
    //     {
    //     case RobotState::MOVING:
    //         //No-Op
    //         break;
            
    //     case RobotState::HORIZONTAL_FEED:
            
    //         break;

    //     case RobotState::VERTICAL_FEED:
            
    //         break;

    //     case RobotState::GROUND_FEED:
            
    //         break;

    //     case RobotState::HORIZONTAL_POLL:
            
    //         break;

    //     case RobotState::VERTICAL_POLL:
            
    //         break;
    //     case RobotState::INVERTED_POLL:
            
    //         break;
    //     case RobotState::LAUNCHING:
            
    //         break;
        
    //     default:
    //         break;
    //     }
    // }
    // else if(ctrl.gachettes.Right >= 0){ //Trigger actions
    //     switch (robot_state)
    //     {
    //     case RobotState::MOVING:
    //         //No-Op
    //         break;
            
    //     case RobotState::HORIZONTAL_FEED:
            
    //         break;

    //     case RobotState::VERTICAL_FEED:
            
    //         break;

    //     case RobotState::GROUND_FEED:
            
    //         break;

    //     case RobotState::HORIZONTAL_POLL:
            
    //         break;

    //     case RobotState::VERTICAL_POLL:
            
    //         break;
    //     case RobotState::INVERTED_POLL:
            
    //         break;
    //     case RobotState::LAUNCHING:
            
    //         break;
        
    //     default:
    //         break;
    //     }
    // }
}

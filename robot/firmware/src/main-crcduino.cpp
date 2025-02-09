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
#include <config.hpp>
//--------------------

//----- Defines ------
#define TICKS_RATIO_BRAS 1425.1 * 2
#define TICKS_RATIO_SWERVE 145.1 * 2.5
#define TICKS_RATIO_LANCE 147

#define MAX_RPM_BRAS 117
#define MAX_RPM_SWERVE 400
#define MAX_RPM_LANCE 1150

#define NB_PM 8
// Index in array (Selon les branchements sur le ALDuino)
#define I_LAS 2
#define I_LBS 3
#define I_RAS 1
#define I_RBS 0
#define I_LB 5
#define I_RB 4
#define I_PB 6
#define I_L 7

// Constants pour la rotation du bras
#define VIT_BRAS_MS M_PI / 2000.0 * 0.1
#define LOW_STOP_BRAS 0.0
#define HIGH_STOP_BRAS 3.14

#define VIT_POIGNET_MS M_PI / 2000.0 * 1.0
#define LOW_STOP_POIGNET -4 // À REVOIR LES LIMITES DU POIGNETS
#define HIGH_STOP_POIGNET 4

enum RobotState
{
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
Controller ctrl;                        // La manette
Timer polling_timer(ONE_SECOND / 1000); // Doit au moins être de 1ms
int32_t df[NB_PM];                      // Le dataframe qui reçoit les valeurs du ALDuino

Motor motors[NB_PM] = {
    {CRC_PWM_7}, // Swerve Right B
    {CRC_PWM_1}, // Swerve Right A
    {CRC_PWM_3}, // Swerve Left A
    {CRC_PWM_4}, // Swerve Left B
    {CRC_PWM_5}, // Bras Right
    {CRC_PWM_6}, // Bras Left
    {CRC_PWM_2}, // Poignet
    {CRC_PWM_8}, // À Déterminé - Lanceur
};

LinEncSpoof spoofs[NB_PM] = {
    {df[0], polling_timer}, // Swerve Right B
    {df[1], polling_timer}, // Swerve Right A
    {df[2], polling_timer}, // Swerve Left A
    {df[3], polling_timer}, // Swerve Left B
    {df[4], polling_timer}, // Bras Right
    {df[4], polling_timer}, // Bras Left  -- On utilise 2 fois le 4 au lieu du 5.
    {df[6], polling_timer}, // Poignet
    {df[7], polling_timer}, // Lanceur
};

GobuildaRotaryEncoder goencs[NB_PM] = {
    {spoofs[0], TICKS_RATIO_SWERVE, polling_timer},       // Swerve Right B
    {spoofs[1], TICKS_RATIO_SWERVE, polling_timer},       // Swerve Right A
    {spoofs[2], TICKS_RATIO_SWERVE, polling_timer, true}, // Swerve Left A
    {spoofs[3], TICKS_RATIO_SWERVE, polling_timer},       // Swerve Left B
    {spoofs[4], TICKS_RATIO_BRAS, polling_timer},         // Bras Right
    {spoofs[4], TICKS_RATIO_BRAS, polling_timer},         // Bras Left
    {spoofs[6], TICKS_RATIO_BRAS, polling_timer},         // Poignet
    {spoofs[7], TICKS_RATIO_LANCE, polling_timer},        // Lanceur
};

PrecisionMotor pmotors[NB_PM] = {
    {"Swerve Right B", motors[0], goencs[0], MAX_RPM_SWERVE}, // Swerve Right B
    {"Swerve Right A", motors[1], goencs[1], MAX_RPM_SWERVE}, // Swerve Right A
    {"Swerve Left A", motors[2], goencs[2], MAX_RPM_SWERVE},  // Swerve Left A
    {"Swerve Left B", motors[3], goencs[3], MAX_RPM_SWERVE},  // Swerve Left B
    {"Bras Right", motors[4], goencs[4], MAX_RPM_BRAS},       // Bras Right
    {"Bras Left", motors[5], goencs[5], MAX_RPM_BRAS},        // Bras Left
    {"Poignet", motors[6], goencs[6], MAX_RPM_BRAS},          // Poignet
    {"Lanceur", motors[7], goencs[7], MAX_RPM_LANCE},         // Lanceur
};

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco_right(CRC_DIG_1, MAX_PULSE_LEN, 0.0, polling_timer); // TODO: get right pin
PwmRotaryEncoder pwm_enco_left(CRC_DIG_2, MAX_PULSE_LEN, 0.0, polling_timer);

SwerveModule swerve_right(pmotors[I_RAS], pmotors[I_RBS], pwm_enco_right);
SwerveModule swerve_left(pmotors[I_LAS], pmotors[I_LBS], pwm_enco_left);

// Pin 9, 10 et 11 PWM sont les pins pour les servos moteurs
Servo servo1;
Servo servo2;
Servo servo3;
//--------------------

//----- Variables ----
unsigned long last_time = millis();
float angle_bras = 0;
float angle_poignet = 0;

bool start_received = false;

//--------------------

//----- Main Prog ----

void setup()
{
    Serial.begin(115200);
    CrcLib::Initialize();

    master_enco_spi_init();
    SPI.begin();

    // TODO: Revoir les configs pour les moteurs aux index 4 et 5 pour les inverted et les PID settings.
    pmotors_config(pmotors);
    servo1.attach(CRC_PWM_9, 1000, 2000);
    servo2.attach(CRC_PWM_10, 1000, 2000);
    servo3.attach(CRC_PWM_11, 1000, 2000);

    for (auto &pmotor : pmotors)
    {
        pmotor._pid_angle.setInterval(polling_timer._delay);
        pmotor.begin();
        pmotor.enable(true);
    }

    Serial.println("End of setup");
}

void loop()
{
    CrcLib::Update();
    ctrl.update();

    polling_timer.update(millis());

    // Hack tant qu'on doivent redémarrer le ALduino!!
    if (!start_received)
    {
        if (ctrl.buttons.Up)
        {
            start_received = true;
        }
        else
        {
            return;
        }
    }

    if (polling_timer.is_time())
    {
        retrieve_df(df);
    }

    // Updates every object
    unsigned long now = millis();
    unsigned long delta = now - last_time;
    last_time = now;

    polling_timer.update(now);

    for (auto &pmotor : pmotors)
    {
        if (pmotor._name == "Poignet" || pmotor._name == "Bras Right" || pmotor._name == "Bras Left")
        {
            pmotor.update();
        }
    }

    if (polling_timer.is_time())
    {
        retrieve_df(df);
        goencs[4].update();
        goencs[5].update();
        Serial.println("df" + String(df[4]) + "   Enco4: " + String(spoofs[4].getLast()) + "      Go4: " + String(goencs[4].getLast().rads));
        Serial.println("df" + String(df[5]) + "   Enco5: " + String(spoofs[5].getLast()) + "      Go5: " + String(goencs[5].getLast().rads));
        Serial.println();
    }

    // ROTATION BRAS
    if (ctrl.gachettes.Left || ctrl.gachettes.Right)
    {
        if (ctrl.gachettes.Left)
        {
            // INWARD
            angle_bras -= ctrl.gachettes.Left * delta * VIT_BRAS_MS;
        }
        else
        {
            // OUTWARD
            angle_bras += ctrl.gachettes.Right * delta * VIT_BRAS_MS;
        }

        // Serial.println(ctrl.gachettes.Right * delta * (M_PI/1000.0));
        // Serial.println(angle_bras);
        angle_bras = constrain(angle_bras, LOW_STOP_BRAS, HIGH_STOP_BRAS);
        pmotors[I_LB].set_target_angle(angle_bras);
        pmotors[I_RB].set_target_angle(angle_bras);

        Serial.println(angle_bras);
    }

    // ROTATION POIGNET
    if (ctrl.buttons.LBumper || ctrl.buttons.RBumper)
    {
        if (ctrl.buttons.LBumper)
        {
            angle_poignet -= delta * VIT_POIGNET_MS;
        }
        else
        {
            angle_poignet += delta * VIT_POIGNET_MS;
        }

        // Serial.println(ctrl.gachettes.Right * delta * (M_PI/1000.0));
        // Serial.println(angle_poignet);
        angle_poignet = constrain(angle_poignet, LOW_STOP_POIGNET, HIGH_STOP_POIGNET);
        pmotors[I_PB].set_target_angle(angle_poignet);

        Serial.println(angle_poignet);
    }

    // GRABBER
    if (ctrl.buttons.A)
    {
        // Serial.println("X");
        servo1.write(2000);
        servo2.write(2000);
        servo3.write(2000);
    }
    else if (ctrl.buttons.B)
    {
        // Serial.println("T");
        servo1.write(1000);
        servo2.write(1000);
        servo3.write(1000);
    }
    else
    {
        // Serial.println("Stop");
        servo1.write(1500);
        servo2.write(1500);
        servo3.write(1500);
    }

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

/*
 * Programme principal du robot
 * Contient la logique de fonctionnement.
 *
 */

//----- Includes -----
#include <Arduino.h>
#include <CrcLib.h>
#include "communication/experimental_controller.hpp"
#include "communication/controller.hpp"
#include "communication/enco_peripherals_uno.hpp"
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
#include <Decodeur.h>
#include "math.h"
//--------------------

//----- Defines ------

#define SWERVE_MODE

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
#define I_RAS 0
#define I_RBS 1
#define I_LB 5
#define I_RB 4
#define I_PB 6
#define I_L 7

// Constants pour la rotation du bras
#define VIT_BRAS_MS M_PI / 4000.0 * 1.2
#define LOW_STOP_BRAS 0.0
#define HIGH_STOP_BRAS 3.14
#define VIT_POIGNET_MS M_PI / 1000.0 * 3
#define LOW_STOP_POIGNET -16 // À REVOIR LES LIMITES DU POIGNETS
#define HIGH_STOP_POIGNET 0

//--------------------

//----- Objects ------
ExController exctrl;
Decodeur cmd(&Serial);

// Timer print_timer(ONE_SECOND / 20);
Timer poll_timer(ONE_SECOND / 20), print_timer(ONE_SECOND / 20);

dataframe_t df;

LinEncSpoof spoofs[ENCO_NUM] = {
    {df[0], poll_timer}, // swerve r a
    {df[1], poll_timer}, // swerv r b
    {df[2], poll_timer}, // arm left
    {df[3], poll_timer}, // launcher
    {df[4], poll_timer}, // wrist
    {df[5], poll_timer}, // arm right
    {df[6], poll_timer}, // swerve l a
    {df[7], poll_timer}, // swerve l b
};

GobuildaRotaryEncoder goencs[ENCO_NUM] = {
    {spoofs[0], 145.1 * 5, poll_timer},
    {spoofs[1], 145.1 * 5, poll_timer},
    {spoofs[2], 145.1 * 5, poll_timer},
    {spoofs[3], 145.1 * 5, poll_timer},
    {spoofs[4], 145.1 * 5, poll_timer},
    {spoofs[5], 145.1 * 5, poll_timer},
    {spoofs[6], 145.1 * 5, poll_timer},
    {spoofs[7], 145.1 * 5, poll_timer},
};

Motor motors[NUM_MOTORS] = {
    {CRC_PWM_4},  // Swerve Right A
    {CRC_PWM_3},  // Swerve Right B
    {CRC_PWM_11}, // Swerve Left A
    {CRC_PWM_12}, // Swerve Left B
    {CRC_PWM_5},  // Bras Right
    {CRC_PWM_2},  // Bras Left
    {CRC_PWM_6},  // Poignet
    {CRC_PWM_1},  // Lanceur
};

PrecisionMotor pmotors[NUM_MOTORS] = {
    {"Swerve Right A", motors[0], goencs[0], MAX_RPM_SWERVE},
    {"Swerve Right B", motors[1], goencs[1], MAX_RPM_SWERVE},
    {"Swerve Left A", motors[2], goencs[6], MAX_RPM_SWERVE},
    {"Swerve Left B", motors[3], goencs[7], MAX_RPM_SWERVE},
    {"Bras Right", motors[4], goencs[5], MAX_RPM_BRAS},
    {"Bras Left", motors[5], goencs[2], MAX_RPM_BRAS},
    {"Poignet", motors[6], goencs[4], MAX_RPM_BRAS},
    {"Lanceur", motors[7], goencs[3], MAX_RPM_LANCE},
};

//----- Variables ----
unsigned long last_time = millis();

bool start_received = false;

//--------------------

/// ---------- TANK DRIVE ----------
void controller_tank_handler()
{
    auto dir = exctrl.joystick_left.xy;
    auto x = dir.x(), y = dir.y();
    auto norm = exctrl.joystick_left.xy.norm();

    if (y == 0)
    {
        set_left(x);
        set_right(-x);
        return;
    }
    if (x == 0)
    {
        set_left(y);
        set_right(y);
        return;
    }
    if (x > 0 and y > 0)
    {
        // top right
        set_left(norm);
        set_right(y);
    }
    if (x < 0 and y > 0)
    {
        // top left
        set_left(y);
        set_right(norm);
    }
    if (x > 0 and y < 0)
    {
        // bottom right
        set_left(-norm);
        set_right(y); // y is negative
    }
    if (x < 0 and y < 0)
    {
        // bottom left
        set_left(y); // y is negative
        set_right(-norm);
    }
}

void set_left(double left_power) {

    const auto SPEED_LEFT = 0.6, SPEED_RIGHT = 0.75;
    pmotors[I_LAS]._m.set_power_ratio(left_power * SPEED_LEFT);
    pmotors[I_LBS]._m.set_power_ratio(-left_power * SPEED_LEFT);
}
void set_right(double right_power) {

    const auto SPEED_LEFT = 0.6, SPEED_RIGHT = 0.75;
    pmotors[I_RAS]._m.set_power_ratio(-right_power * SPEED_RIGHT);
    pmotors[I_RBS]._m.set_power_ratio(right_power * SPEED_RIGHT);
}

/// ---------- Main Program ---------
void setup()
{
    /// ########## Initialisation ###########
    Serial.begin(115200);
    CrcLib::Initialize();
    exctrl.begin();

    /// ########## PMotors Configs ###########
    for (auto &pmotor : pmotors)
    {
        pmotor._pid_angle.setInterval(poll_timer._delay);
        pmotor._pid_speed.setInterval(poll_timer._delay);
        pmotor.begin();
        pmotor.enable(true); // TODO REMEMBER TO BEGIN AND ENABLE THE ARMS, WRIST AND LAUNCHER SEPARATELY
    }
    pmotors_config(pmotors);

    Serial.println("End of setup");
}

void loop()
{
    // ########## Generic Updates ###########
    CrcLib::Update();
    exctrl.update();

    unsigned long now = millis();
    poll_timer.update(now);
    print_timer.update(now);

    /// ########## Printing ###########
    if (print_timer.is_time())
    {
        // Serial.print(pmotors[I_L]._e.getLast().rpm);
        Serial.println("X" + String(exctrl.joystick_left.angle));
    }
    /// ########## Controller Input ###########

    controller_tank_handler();
}

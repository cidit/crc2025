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

// #define SWERVE_MODE

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
Controller ctrl;
ExController exctrl;

// FIXME: the tunings for the angles use this interval. change those
// Timer polling_timer(ONE_SECOND / 1000); // Doit au moins être de 1ms

Decodeur cmd(&Serial);

// Timer print_timer(ONE_SECOND / 20);
Timer poll_timer(ONE_SECOND / 20), print_timer(ONE_SECOND/20);

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

const auto MAX_PULSE_LEN = 4160.0;
PwmRotaryEncoder pwm_enco_left(CRC_DIG_1, MAX_PULSE_LEN, -1.07, poll_timer);
PwmRotaryEncoder pwm_enco_right(CRC_DIG_2, MAX_PULSE_LEN, -2.10, poll_timer);

SwerveModule swerve_right(pmotors[I_RAS], pmotors[I_RBS], pwm_enco_right);
SwerveModule swerve_left(pmotors[I_LAS], pmotors[I_LBS], pwm_enco_left);

SwerveDrive swerve_drive(swerve_right, swerve_left);

Servo servo_manipulator_1; // PWM11
Servo servo_manipulator_2; // PWM12
Servo servo_manipulator_3; // PWM3
Servo servo_lanceur;       // PWM2
//--------------------

//----- Variables ----
unsigned long last_time = millis();
float angle_bras = 0;
float angle_poignet = 0;

bool start_received = false;
bool launcherOn = false;

//--------------------

//----- Main Prog ----

void controller_misc_handler()
{
    if (exctrl.start.isPressed())
    // if (exctrl._raw.start)
    {
        alduino_reset();
    }
    // colors up down for arm
    // L1, L2 for wrist
    // color right left for eating spitting

    // hatr -> activate thrower
}

void controller_arms_handler()
{
    auto now = millis();
    unsigned long delta = now - last_time;
    last_time = now;

    // ####### ROTATION BRAS #######
    if (exctrl.left_trigger || exctrl.right_trigger)
    {
        if (exctrl.left_trigger)
        {
            // INWARD
            angle_bras -= powf(2, 1.0+exctrl.left_trigger) * delta * VIT_BRAS_MS;
        }
        else
        {
            // OUTWARD
            angle_bras += powf(2, 1.0+exctrl.right_trigger) * delta * VIT_BRAS_MS;
        }

        // Serial.println(ctrl.gachettes.Right * delta * (M_PI/1000.0));
        // Serial.println(angle_bras);
        // angle_bras = constrain(angle_bras, LOW_STOP_BRAS, HIGH_STOP_BRAS);
        pmotors[I_LB].set_target_angle(angle_bras);
        // pmotors[I_LB].set_target_angle(angle_bras);
        pmotors[I_RB].set_target_angle(angle_bras);

        Serial.println(angle_bras);
    }
    // #############################

    // ##### ROTATION POIGNET ######
    // if (ctrl.color_up.isOnPress() || ctrl.color_down.isOnPress())
    if (ctrl.buttons.A || ctrl.buttons.Y)
    {
        if (ctrl.buttons.A)
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
    // #############################

    // ########## GRABBER ##########
    if (ctrl.buttons.RBumper)
    {
        Serial.println("X");
        servo_manipulator_1.write(2000);
        servo_manipulator_2.write(2000);
        servo_manipulator_3.write(2000);
    }
    else if (ctrl.buttons.LBumper)
    {
        Serial.println("T");
        servo_manipulator_1.write(1000);
        servo_manipulator_2.write(1000);
        servo_manipulator_3.write(1000);
    }
    else
    {
        servo_manipulator_1.write(1500);
        servo_manipulator_2.write(1500);
        servo_manipulator_3.write(1500);
    }
}

// void controller_swerve_handler()
// {

//     auto howmuch_to_turn = ectrl.joystick_right.xy.x();
//     auto translation = ctrl.joystick_left.xy;

//     swerve_drive.set_target({.heading = {
//                                  .direction = translation.angle(),
//                                  .velocity = translation.norm() * 100,
//                              },
//                              .rotation = howmuch_to_turn * 100});
// }

void controller_tank_handler()
{

    auto dir = exctrl.joystick_left.xy;

    if (dir.norm() < EPSILON)
    {
        pmotors[I_LAS]._m.set_power_ratio(0);
        pmotors[I_LBS]._m.set_power_ratio(0);
        pmotors[I_RAS]._m.set_power_ratio(0);
        pmotors[I_RBS]._m.set_power_ratio(0);
        return;
    }

    double sinus = dir.norm() * sinf(dir.angle()); //y
    //double left_pow  = dir.norm() * sinf(dir.angle());


    double right_power, left_power;

    // LEFTSIDE
    if (dir.angle() >= Angle::from_rad(0)._radians &&
        dir.angle() <= Angle::from_rad(M_PI_2)._radians)
    {
        // top right
        left_power = 1;
        right_power = map(sinus*10, 0, 10, -10, 10)/10.0;
    }
    else if (dir.angle() >= Angle::from_rad(M_PI_2)._radians &&
        dir.angle() <= Angle::from_rad(M_PI)._radians)
    {
        // top left
        right_power = 1;
        left_power = map(sinus*10, 0, 10, -10, 10)/10.0;

    }
    else if (dir.angle() >= Angle::from_rad(M_PI)._radians &&
             dir.angle() <= Angle::from_rad(M_PI_2 + M_PI)._radians)
    {
        // bottom left
        left_power = map(sinus*10, -10, 0, -10, 10)/10.0;
        right_power = -1;
    }
    else if (dir.angle() >= Angle::from_rad(M_PI_2 + M_PI)._radians &&
        dir.angle() <= Angle::from_rad(2 * M_PI)._radians)
    {
        // bottom right
        left_power = -1.0;
        right_power = map(sinus*10, -10, 0, -10, 10)/10.0;
    }

    const auto SPEED_LEFT = 0.75, SPEED_RIGHT = 0.75;
    right_power *= dir.norm() * SPEED_LEFT;
    left_power *= dir.norm() * SPEED_RIGHT;


    pmotors[I_LAS]._m.set_power_ratio(left_power);
    pmotors[I_LBS]._m.set_power_ratio(-left_power);
    pmotors[I_RAS]._m.set_power_ratio(-right_power);
    pmotors[I_RBS]._m.set_power_ratio(right_power);
}

void controller_launcher_handler()
{
    // start laucher spin
    if (ctrl.buttons.X)
    {
        launcherOn = !launcherOn;
    }
    
    if(launcherOn)
        pmotors[I_L]._m.set_power_ratio(-0.45);
    else
        pmotors[I_L]._m.set_power_ratio(0);

    // feed laucher
    if (ctrl.buttons.B)
    {
        Serial.println("feeding");
        servo_lanceur.write(80);
    }
    if (!ctrl.buttons.B)
    {
        servo_lanceur.write(180);
    }
}

void update_arms()
{
    for (auto &pmotor : pmotors)
    {
        // Il ne faut pas updater le moteur gauche du bras (I_LB), il ne faut pas utiliser 2 PID
        if (pmotor._name == "Poignet" || pmotor._name == "Bras Right" || pmotor._name == "Lanceur")
        { //|| pmotor._name == "Bras Left"
            pmotor.update();
        }
    }

    // Update le moteur dont on utilise pas le PID // KEEP ARMS IN SYNC
    pmotors[I_LB]._m.set_power_ratio(pmotors[I_RB]._pid_angle.getOutput());
}

void update_launcher()
{
    // TODO: do this
    pmotors[I_L].update();
}

void update_tank()
{
    // pmotors[I_LAS].update();
    // pmotors[I_LBS].update();
    // pmotors[I_RAS].update();
    // pmotors[I_RBS].update();
}

void setup()
{
    // --- INITIALISATION SECTION ---
    Serial.begin(115200);
    CrcLib::Initialize();
    ctrl.begin();
    exctrl.begin();

    master_enco_spi_init();
    SPI.begin(); // if (ctrl._raw.start)
    // {
    //     alduino_reset();
    // }t les PID settings.
    servo_manipulator_1.attach(CRC_PWM_7, 1000, 2000);
    servo_manipulator_2.attach(CRC_PWM_9, 1000, 2000);
    servo_manipulator_3.attach(CRC_PWM_10, 1000, 2000);
    servo_lanceur.attach(CRC_PWM_8); // TODO: pas 360

    alduino_reset();
    // swerve_drive.ben();

    // --- CONFIG SECTION ---
    for (auto &pmotor : pmotors)
    {
        pmotor._pid_angle.setInterval(poll_timer._delay);
        pmotor._pid_speed.setInterval(poll_timer._delay);
        pmotor.begin();
        pmotor.enable(true); // TODO REMEMBER TO BEGIN AND ENABLE THE ARMS, WRIST AND LAUNCHER SEPARATELY
    }
    pmotors_config(pmotors);
    {
        swerve_drive._r._pid.setInterval(poll_timer._delay);
        swerve_drive._l._pid.setInterval(poll_timer._delay);
    }
    // swerve_config(swerve_drive);
    // swerve_drive.enable(true);

    // TODO: enable arms n launcher

    Serial.println("End of setup");
}

void loop()
{
    // ########## UPDATE ###########
    CrcLib::Update();
    ctrl.update();
    exctrl.update();

    unsigned long now = millis();
    poll_timer.update(now);
    print_timer.update(now);
    
    if (print_timer.is_time()) {
        Serial.print(pmotors[I_L]._e.getLast().rpm);
    }

    // servo_manipulator_1.writeMicroseconds(2000);
    // //servo_manipulator_2.writeMicroseconds(2000);
    // //servo_manipulator_3.writeMicroseconds(2000);
    // return ;

    if (poll_timer.is_time())
    {
        retrieve_df(df);
        for (auto &go : goencs)
        {
            go.update();
        }
        // goencs[4].update(); // TODO: should not be necessary. should happen automatically when the pmotors update below -f
        // goencs[5].update();
    }

    controller_misc_handler();
    controller_arms_handler();
    controller_launcher_handler();
#ifdef SWERVE_MODE
    // controller_swerve_handler();
#else
    controller_tank_handler();
#endif

    /// UPDATE DEVICES

    update_arms();
    update_launcher();
#ifdef SWERVE_MODE
    // swerve_drive.update();
#else
    update_tank();
#endif
}

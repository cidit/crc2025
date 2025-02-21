// W.I.P.

// requires:
// - [X] swerve drive implementation to test
// - [ ] controller implementation

/*
spi master. expects a slave to be uploaded to the arduino.
*/

#include <Arduino.h>
#include <SPI.h>
#include <Encoder.h>
#include <Servo.h>
#include <Decodeur.h>
#include <PID_RT.h>
#include <CrcLib.h>
#include <sensors/lin_enco_spoof.hpp>
#include <sensors/gobuilda_rotary_enc.hpp>
#include "communication/enco_peripherals_uno.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print_crc_extras.hpp"
#include "util/looped.hpp"
#include <drives/precision_motor.hpp>
#include <drives/swerve_module.hpp>
#include "math/vectors.hpp"
#include "config.hpp"
#include "communication/experimental_controller.hpp"

Decodeur cmd(&Serial);
ExController ctlr;
bool read_mode = true;
Timer print_timer(ONE_SECOND / 20);
Timer poll_timer(ONE_SECOND / 20);
Timer swerve_timer(ONE_SECOND / 20);
Timer heartbeat_timer(3 * ONE_SECOND);

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

bool started = false;

void execute_commands()
{
    // no-op, for now
}

void setup()
{
    Serial.begin(115200);
    CrcLib::Initialize();
    ctlr.begin();
    master_enco_spi_init();
    SPI.begin();

    // fixing the same interval for the motors as configuring them.
    for (auto &pmotor : pmotors)
    {
        pmotor._pid_angle.setInterval(poll_timer._delay);
        pmotor._pid_speed.setInterval(poll_timer._delay);
    }
    pmotors_config(pmotors);

    {
        swerve_drive._r._pid.setInterval(swerve_timer._delay);
        swerve_drive._l._pid.setInterval(swerve_timer._delay);
    }
    swerve_config(swerve_drive);

    alduino_reset();

    swerve_drive.begin();
    swerve_drive.enable(true);
    Serial.println("Setup Done");
}

void loop()
{
    auto now = millis();
    print_timer.update(now);
    poll_timer.update(now);
    swerve_timer.update(now);
    heartbeat_timer.update(now);
    CrcLib::Update();
    ctlr.update();
    cmd.refresh();

    if (ctlr._raw.select)
    // if (exctrl._raw.start)
    {
        alduino_reset();
    }

    if (ctlr._raw.start) {
        Serial.println("out!");
        started = true;
    }

    if (!started) {
        return;
    }

    execute_commands();

    if (poll_timer.is_time())
    {
        retrieve_df(df);
    }

    if (heartbeat_timer.is_time())
    {
        SPRINT('\n');
    }

    // colors up do
    auto howmuch_to_turn = ctlr.joystick_right.xy.x();
    auto translation = ctlr.joystick_left.xy;

    if (print_timer.is_time())
    {
        // pwm_enco_left.update();
        // pwm_enco_right.update();
        if (!CrcLib::IsCommValid())
        {
            Serial.println("no com");
        }
        SEPARATOR;
        print_battery();
        SEPARATOR;

        SPRINT("L: ");
        SPRINT(pwm_enco_left.getLast().rads);
        SPRINT(" [");
        SPRINT(swerve_drive._l._target.direction);
        SPRINT(", ");
        SPRINT(swerve_drive._l._target.velocity);
        SPRINT("]");
        SEPARATOR;

        SPRINT("R: ");
        SPRINT(pwm_enco_right.getLast().rads);
        SPRINT(" [");
        SPRINT(swerve_drive._r._target.direction);
        SPRINT(", ");
        SPRINT(swerve_drive._r._target.velocity);
        SPRINT("]");
        SEPARATOR;

        SPRINT("HMTT:" + String(howmuch_to_turn));
        SPRINT(" X:" + String(translation.x()));
        SPRINT(" Y:" + String(translation.y()));

        Serial.println();
    }

    swerve_drive.set_target({.heading = {
                                 .direction = translation.angle(),
                                 .velocity = translation.norm() * 100,
                             },
                             .rotation = howmuch_to_turn * 100});
    swerve_drive.update();
}
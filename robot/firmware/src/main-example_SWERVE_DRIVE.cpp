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
    {df[0], poll_timer}, // swerve right b
    {df[1], poll_timer}, // swerve right a
    {df[2], poll_timer}, // swerve left a
    {df[3], poll_timer}, // swerve left b
    {df[4], poll_timer},
    {df[5], poll_timer},
    {df[6], poll_timer},
    {df[7], poll_timer},
};

GobuildaRotaryEncoder goencs[ENCO_NUM] = {
    {spoofs[0], 145.1 * 5, poll_timer},
    {spoofs[1], 145.1 * 5, poll_timer},
    {spoofs[2], 145.1 * 3.55, poll_timer},
    {spoofs[3], 145.1 * 3.55, poll_timer},
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
PwmRotaryEncoder pwm_enco_left(CRC_DIG_2, MAX_PULSE_LEN, -1.07, swerve_timer);
PwmRotaryEncoder pwm_enco_right(CRC_DIG_1, MAX_PULSE_LEN, -2.10, swerve_timer);

SwerveModule swerve_right(
    pmotors[1],
    pmotors[0],
    pwm_enco_right);

SwerveModule swerve_left(
    pmotors[2],
    pmotors[3],
    pwm_enco_left);

SwerveDrive swerve_drive(swerve_right, swerve_left);

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

    swerve_drive.begin();
    {
        swerve_drive._r._pid.setInterval(swerve_timer._delay);
        swerve_drive._l._pid.setInterval(swerve_timer._delay);
    }
    swerve_config(swerve_drive);

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
    execute_commands();

    if (poll_timer.is_time())
    {
        retrieve_df(df);
    }

    if (heartbeat_timer.is_time())
    {
        SPRINT('\n');
    }

    if (ctlr.start.isPressed())
    // if (exctrl._raw.start)
    {
        alduino_reset();
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

        // SPRINT(" L:");
        // SPRINT(" A:");
        // SPRINT(ctlr.joystick_left.angle);
        // SPRINT(" N:");
        // SPRINT(ctlr.joystick_left.xy.norm());
        // SPRINT(" T:");
        // SPRINT(ctlr.left_trigger);
        // SPRINT(" B:");
        // SPRINT(ctlr.left_bumper.isPressed());
        // SEPARATOR;

        // SPRINT(" R:");
        // SPRINT(" A:");
        // SPRINT(ctlr.joystick_right.angle);
        // SPRINT(" N:");
        // SPRINT(ctlr.joystick_right.xy.norm());
        // SPRINT(" T:");
        // SPRINT(ctlr.right_trigger);
        // SPRINT(" B:");
        // SPRINT(ctlr.right_bumper.isPressed());
        // SEPARATOR;

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
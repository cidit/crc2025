/*
spi master. expects a slave to be uploaded to the arduino.
*/

#include <Arduino.h>
#include <SPI.h>
#include <sensors/lin_enco_spoof.hpp>
#include <sensors/gobuilda_rotary_enc.hpp>
#include "communication/enco_peripherals_uno.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print_crc_extras.hpp"
#include "Decodeur.h"

Timer poll_timer(ONE_SECOND / 20);
Decodeur dcd(&Serial);

int32_t df[ENCO_NUM];

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

bool reset_not_done = true;

void setup()
{
    Serial.begin(115200);
    master_enco_spi_init();
    SPI.begin();

    alduino_reset();
}

void loop()
{
    dcd.refresh();

    if (dcd.isAvailable())
    {
        if (dcd.getCommand() == '\'')
        {
            alduino_reset();
        }
    }

    if (millis() > 1000 && reset_not_done)
    {
        alduino_reset();
        reset_not_done = false;
    }

    poll_timer.update(millis());

    if (poll_timer.is_time())
    {
        retrieve_df(df);
    }

    // TESTING GOBILDA ENCS
    if (poll_timer.is_time())
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
}

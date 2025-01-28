/*
spi master. expects a slave to be uploaded to the arduino.
*/

#include <Arduino.h>
#include <SPI.h>
#include <sensors/lin_enco_spoof.hpp>
#include <sensors/gobuilda_rotary_enc.hpp>
#include "communication/enco_peripherals.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print.hpp"

Timer poll_timer(ONE_SECOND / 10000);

int32_t df[ENCO_NUM];

LinEncSpoof spoofs[ENCO_NUM] = {
    {df[0], poll_timer},
    {df[1], poll_timer},
    {df[2], poll_timer},
    {df[3], poll_timer},
    {df[4], poll_timer},
    {df[5], poll_timer},
    {df[6], poll_timer},
    {df[7], poll_timer},
};

GobuildaRotaryEncoder goencs[ENCO_NUM] = {
    {spoofs[0], 145.1 * 2.5, poll_timer},
    {spoofs[1], 145.1 * 2.5, poll_timer},
    {spoofs[2], 145.1 * 2.5, poll_timer},
    {spoofs[3], 145.1 * 2.5, poll_timer},
    {spoofs[4], 145.1 * 2.5, poll_timer},
    {spoofs[5], 145.1 * 2.5, poll_timer},
    {spoofs[6], 145.1 * 2.5, poll_timer},
    {spoofs[7], 145.1 * 2.5, poll_timer},
};

void setup()
{
    Serial.begin(115200);
    master_enco_spi_init();
    SPI.begin();
}

void update_df()
{
    // TODO: for some reason, the values seem to be multiplied by 256
    retrieve_df(df);
    // here, we sorta patch the multiplied by 256 problem
    for (int i = 0; i < ENCO_NUM; i++)
    {
        df[i] /= 256;
    }
}

void loop()
{
    poll_timer.update(millis());

    if (poll_timer.is_time())
    {
        update_df();
    }

    // // TESTING DATAFRAME
    // if (poll_timer.is_time())
    // {
    //     for (auto i = 0; i < ENCO_NUM; i++)
    //     {
    //         SPRINT("| e");
    //         SPRINT(i);
    //         SPRINT(":");
    //         SPRINT(df[i]);
    //         SPRINT(" ");
    //     }
    //     Serial.println("|");
    // }


    // // TESTING SPOOFS
    // if (poll_timer.is_time())
    // {
    //     for (auto i = 0; i < ENCO_NUM; i++)
    //     {
    //         spoofs[i].update();
    //         SPRINT("| e");
    //         SPRINT(i);
    //         SPRINT(":");
    //         SPRINT(spoofs[i].getLast());
    //         SPRINT(" ");
    //     }
    //     Serial.println("|");
    // }

    // TESTING GOBILDA ENCS
    if (poll_timer.is_time())
    {
        for (auto i = 0; i < ENCO_NUM; i++)
        {
            goencs[i].update();
            SPRINT("| e");
            SPRINT(i);
            SPRINT(" ");
            SPRINT(goencs[i].getLast().rads);
            SPRINT(" ");
            SPRINT(goencs[i].getLast().rpm);
            SPRINT(" ");
        }
        Serial.println("|");
    }
}

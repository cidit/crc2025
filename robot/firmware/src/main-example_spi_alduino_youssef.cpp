
#include "communication/youssef_was_right.hpp"

#include <Arduino.h>
#include <SPI.h>
#include "util/print.hpp"
#include "communication/enco_peripherals.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "sensors/gobuilda_rotary_enc_data.hpp"


Timer print_timer(ONE_SECOND);

Encoder e[ENCO_NUM] = {
    Encoder(38, 36), // right high
    Encoder(40, 34), // right low (ITS INVERTED!)
    Encoder(42, 32), // left low
    Encoder(44, 30), // left high
    Encoder(46, 28),
    Encoder(48, 26),
    Encoder(50, 24),
    Encoder(52, 22)};


dataframe_t df;

void setup() {
    Serial.begin(115200);
    spi_setup();
}

void loop() {
    print_timer.update(millis());

    transmit_df(df);
    
    for (byte i = 0; i < ENCO_NUM; i++)
    {
        df[i] = e[i].read();
    }

    if (print_timer.is_time())
    {
        SPRINT(" ");
        for (byte i = 0; i < ENCO_NUM; i++)
        {
            SPRINT("| e");
            SPRINT(i);
            SPRINT(":");
            SPRINT(df[i]);
            SPRINT(" ");
        }
        Serial.println('|');
    }
}
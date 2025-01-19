/*
spi master
*/


#include <Arduino.h>
#include <SPI.h>
#include "communication/enco_peripherals.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print.hpp"

Timer spi_poll_timer(ONE_SECOND/1);

void myinit() {
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
}

dataframe retrieve_df() {
    dataframe_softcast dfs;

    SPI.beginTransaction(SPI_AL_CRC_SETTINGS);
    digitalWrite(SS, LOW); // INITIATE COMM WITH SLAVE
    for (uint8_t i = 0; i < sizeof(dfs.bytes); i++) {
        dfs.bytes[i] = SPI.transfer(i);
    }
    digitalWrite(SS, HIGH); // END COMM
    SPI.endTransaction();

    return dfs.data;
}

void setup() {
    Serial.begin(115200);
    
    myinit();
    SPI.begin();
    // SPI.begin(SS);
    // SPI.setDataMode(SS, SPI_MODE0);
    // SPI.setClockDivider(SS, SPI_CLOCK_DIV8); // ajust to match clock speed with master
}

void loop() {
    spi_poll_timer.update(millis());

    if (spi_poll_timer.is_time()) {
        auto df = retrieve_df();
        for (auto data: df.data) {
            SPRINT("v:" + String(data.rpm));
            SPRINT("a:"+ String(data.rads));
            SEPARATOR;
        }
        SPRINT('\n');
    }
    
}


#include "communication/youssef_was_right.hpp"

#include <Arduino.h>
#include <SPI.h>
#include "communication/enco_peripherals.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print.hpp"

Timer poll_timer(ONE_SECOND/1);
dataframe_t df;

void setup() {
    Serial.begin(115200);
    spi_setup();
}

void loop() {

    poll_timer.update(millis());

    if (poll_timer.is_time()) {
        int32_t df[ENCO_NUM];
        update_df(df);
        for (auto i = 0; i < ENCO_NUM; i++) {
            SPRINT("| e");
            SPRINT(i);
            SPRINT(":");
            SPRINT(df[i]);
            SPRINT(" ");
        }
        Serial.println("|");
        auto raw = reinterpret_cast<byte*>(df);
        for (size_t i = 0; i < DATAFRAME_BUFFER_LEN; i++) {
            if (i%4==0) Serial.print(" ");
            Serial.print(raw[i], HEX);
        }
        Serial.println();
    }
}
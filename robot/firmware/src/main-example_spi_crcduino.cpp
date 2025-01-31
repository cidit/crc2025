// /*
// spi master
// */


#include <Arduino.h>
#include <SPI.h>
#include "communication/enco_peripherals.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print.hpp"


Timer poll_timer(ONE_SECOND/1);

void setup() {
    Serial.begin(115200);
    master_enco_spi_init();
    SPI.begin();
}

void loop() {
    poll_timer.update(millis());

    if (poll_timer.is_time()) {
        dataframe_t df;
        retrieve_df(df);
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
            if (raw[i] < 10) Serial.print("0");
            Serial.print(raw[i], HEX);
        }
        Serial.println("");
    }
    
}

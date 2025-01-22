#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "sensors/gobuilda_rotary_enc_data.hpp"

static const SPISettings SPI_AL_CRC_SETTINGS(
    1000000,
    MSBFIRST,
    SPI_MODE0);

constexpr auto ENCO_NUM = 8;
constexpr auto DF_LEN = sizeof(int32_t) * ENCO_NUM;


void master_enco_spi_init() {
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
}

/**
 * retrieves a dataframe
 */
void retrieve_df(int32_t df[ENCO_NUM]) {
    auto buff = (byte*)df;

    SPI.beginTransaction(SPI_AL_CRC_SETTINGS);
    digitalWrite(SS, LOW); // INITIATE COMM WITH SLAVE

    for (byte i = 0; i < DF_LEN; i++) {
      buff[i] = SPI.transfer(0x01); // 0x01 is just random data. we need to send something to receive something.
    }

    digitalWrite(SS, HIGH); // END COMM
    SPI.endTransaction();
}
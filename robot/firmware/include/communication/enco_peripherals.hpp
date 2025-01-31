#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "sensors/gobuilda_rotary_enc_data.hpp"


constexpr size_t NUM_ENCOS = 8;
constexpr size_t DATAFRAME_BUFFER_LEN = (sizeof(int32_t) * NUM_ENCOS);

using dataframe_t = int32_t[NUM_ENCOS];

static const SPISettings SPI_AL_CRC_SETTINGS(
    1000000,
    MSBFIRST,
    SPI_MODE0);

constexpr auto ENCO_NUM = 8;
constexpr auto DF_LEN = sizeof(int32_t) * ENCO_NUM;


#ifndef ARDUINO_SAM_DUE

void master_enco_spi_init() {
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
}

/**
 * retrieves a dataframe.
 * the dataframe should always be updated before it's dependents are themselves updated
 */
void retrieve_df(int32_t df[ENCO_NUM]) {
    auto buff = (byte*)df;

    SPI.beginTransaction(SPI_AL_CRC_SETTINGS);
    digitalWrite(SS, LOW); // INITIATE COMM WITH SLAVE
    
    SPI.transfer(0x0F); // sync
    // SPI.transfer(0x00); // trash the first byte i guess???

    for (byte i = 0; i < DF_LEN; i++) {
      buff[i] = SPI.transfer(0x00); // 0x00 is just random data. we need to send something to receive something.
    }

    digitalWrite(SS, HIGH); // END COMM
    SPI.endTransaction();
}


#endif
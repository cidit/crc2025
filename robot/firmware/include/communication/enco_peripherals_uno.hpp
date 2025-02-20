#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "sensors/gobuilda_rotary_enc_data.hpp"
#include "enco_peripherals_common.hpp"

#include <CrcLib.h>

constexpr auto ALDUINO_RESET = CRC_DIG_10;

void alduino_reset()
{
    // CrcLib::SetDigitalOutput()
    Serial.println("alduino RESET");
    delay(100);
    digitalWrite(ALDUINO_RESET, LOW);
    delay(100);
    digitalWrite(ALDUINO_RESET, HIGH);
}

void alduino_reset_via_spi() {

    SPI.beginTransaction(SPI_AL_CRC_SETTINGS);
    digitalWrite(SS, LOW); // INITIATE COMM WITH SLAVE

    SPI.transfer(RESET_BYTE);
    // SPI.transfer(0x00); // trash the first byte i guess???

    digitalWrite(SS, HIGH); // END COMM
    SPI.endTransaction();
}

void master_enco_spi_init()
{
    pinMode(ALDUINO_RESET, OUTPUT);
    digitalWrite(ALDUINO_RESET, HIGH);
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
}

/**
 * retrieves a dataframe.
 * the dataframe should always be updated before it's dependents are themselves updated
 */
void retrieve_df(int32_t df[ENCO_NUM])
{
    auto buff = (byte *)df;

    SPI.beginTransaction(SPI_AL_CRC_SETTINGS);
    digitalWrite(SS, LOW); // INITIATE COMM WITH SLAVE

    SPI.transfer(SYNC_BYTE); // sync
    // SPI.transfer(0x00); // trash the first byte i guess???

    for (byte i = 0; i < DF_LEN; i++)
    {
        buff[i] = SPI.transfer(0x00); // 0x00 is just random data. we need to send something to receive something.
    }

    digitalWrite(SS, HIGH); // END COMM
    SPI.endTransaction();
}
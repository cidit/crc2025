#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "sensors/gobuilda_rotary_enc_data.hpp"

constexpr auto ENCO_NUM = 8;
using dataframe_t = int32_t[ENCO_NUM];
constexpr auto DF_LEN = sizeof(dataframe_t);

uint8_t SYNC_BYTE = 0x0F, RESET_BYTE = 0xFF;

void hexdump_df(dataframe_t &df)
{
    auto raw = reinterpret_cast<byte *>(df);
    for (size_t i = 0; i < DF_LEN; i++)
    {
        if (i % 4 == 0)
            Serial.print(" ");
        if (raw[i] < 10)
            Serial.print("0");
        Serial.print(raw[i], HEX);
    }
}

static const SPISettings SPI_AL_CRC_SETTINGS(
    100000,
    MSBFIRST,
    SPI_MODE0);


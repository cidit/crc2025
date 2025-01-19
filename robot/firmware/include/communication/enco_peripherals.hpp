#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "sensors/gobuilda_rotary_enc_data.hpp"

static const SPISettings SPI_AL_CRC_SETTINGS(
    1000000,
    MSBFIRST,
    SPI_MODE0);

struct dataframe
{
    GobuildaRotaryEncoderData data[8];
};

union dataframe_softcast
{
    dataframe data;
    byte bytes[sizeof(dataframe)]; // + 1]; // +1 is necessary because SPI works in alternance
};

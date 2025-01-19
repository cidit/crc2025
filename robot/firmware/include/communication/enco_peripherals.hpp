#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "sensors/gobuilda_rotary_enc.hpp"

static const SPISettings SPI_AL_CRC_SETTINGS(
    1000000,
    MSBFIRST,
    SPI_MODE0);

typedef struct dataframe
{
    GobuildaRotaryEncoderData data[8];
};

union dataframe_softcast
{
    dataframe data;
    byte bytes[sizeof(dataframe)];
};

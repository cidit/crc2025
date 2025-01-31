// /*
// spi slave
// */

#include <Arduino.h>
#include <SPI.h>
#include "util/print.hpp"
#include "communication/enco_peripherals.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "sensors/gobuilda_rotary_enc_data.hpp"

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

int df_idx = 0;

void setup()
{
    Serial.begin(115200);
    spi_slave_crazy_init();
}

void loop()
{
    // update dataframe
    for (byte i = 0; i < ENCO_NUM; i++)
    {
        df[i] = e[i].read();
    }
}

void SPI0_Handler()
{
    byte read_value = SPI0->SPI_RDR & SPI_RDR_RD_Msk;
    if (read_value == 0x0F) {
        df_idx = 0;
    }

    auto raw = reinterpret_cast<byte*>(df);
    SPI0->SPI_TDR = (raw[df_idx]);

    if (++df_idx == DF_LEN)
    {
        df_idx = 0;
    }
}

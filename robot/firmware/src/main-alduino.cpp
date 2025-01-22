// /*
// spi slave
// */

// #include <ArduinoSTL.h>
#include <Arduino.h>
#include <SPI.h>
#include <Encoder.h>
#include "communication/enco_peripherals.hpp"

bool does_master_need_data()
{
    return digitalRead(SS) == LOW;
}

void spi_slave_crazy_init()
{
    // FROM: https://github.com/MrScrith/arduino_due/blob/master/spi_slave.ino
    NVIC_ClearPendingIRQ(SPI0_IRQn);
    NVIC_EnableIRQ(SPI0_IRQn);
    SPI.begin(SS);
    REG_SPI0_CR = SPI_CR_SWRST; // reset SPI

    // Setup interrupt
    REG_SPI0_IDR = SPI_IDR_TDRE | SPI_IDR_MODF | SPI_IDR_OVRES | SPI_IDR_NSSR | SPI_IDR_TXEMPTY | SPI_IDR_UNDES;
    REG_SPI0_IER = SPI_IER_RDRF;

    // Setup the SPI registers.
    REG_SPI0_CR = SPI_CR_SPIEN;   // enable SPI
    REG_SPI0_MR = SPI_MR_MODFDIS; // slave and no modefault
    REG_SPI0_CSR = SPI_MODE0;     // DLYBCT=0, DLYBS=0, SCBR=0, 8 bit transfer
}

Encoder e[ENCO_NUM] = {
    Encoder(38, 36), // right high
    Encoder(40, 34), // right low (ITS INVERTED!)
    Encoder(42, 32), // left low
    Encoder(44, 30), // left high
    Encoder(46, 28),
    Encoder(48, 26),
    Encoder(50, 24),
    Encoder(52, 22)};

byte df[sizeof(int32_t) * ENCO_NUM];

int df_idx = 0;

void setup()
{
    Serial.begin(115200);
    spi_slave_crazy_init();
}

void loop()
{
    // update dataframe
    auto df_enco_vals = (int32_t *)df;
    for (byte i = 0; i < ENCO_NUM; i++)
    {
        df_enco_vals[i] = e[i].read();
    }
}

void SPI0_Handler()
{
    // MUST READ EVEN IF UNUSED
    // how it would normally be: `byte read_value = SPI0->SPI_RDR & SPI_RDR_RD_Msk;`
    (void) (SPI0->SPI_RDR & SPI_RDR_RD_Msk);
    SPI0->SPI_TDR = (df[df_idx]);

    if (++df_idx == DF_LEN)
    {
        df_idx = 0;
    }
}
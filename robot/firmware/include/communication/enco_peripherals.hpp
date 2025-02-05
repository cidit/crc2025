#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "sensors/gobuilda_rotary_enc_data.hpp"

constexpr auto ENCO_NUM = 8;
using dataframe_t = int32_t[ENCO_NUM];
constexpr auto DF_LEN = sizeof(dataframe_t);

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
    500000,
    MSBFIRST,
    SPI_MODE0);

#ifndef ARDUINO_SAM_DUE

void master_enco_spi_init()
{
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

    SPI.transfer(0x0F); // sync
    // SPI.transfer(0x00); // trash the first byte i guess???

    for (byte i = 0; i < DF_LEN; i++)
    {
        buff[i] = SPI.transfer(0x00); // 0x00 is just random data. we need to send something to receive something.
    }

    digitalWrite(SS, HIGH); // END COMM
    SPI.endTransaction();
}
#else

void spi_slave_crazy_init()
{
    // FROM: https://forum.arduino.cc/t/arduino-due-spi-interrupt/574857/4

    // // SPI serial recieve
    // REG_PMC_PCER0 |= PMC_PCER0_PID24;   // Power up SPI clock
    // REG_SPI0_WPMR = 0 << SPI_WPMR_WPEN; // Unlock user interface for SPI

    // // Instance SPI0, MISO: PA25, (MISO), MOSI: PA26, (MOSI), SCLK: PA27, (SCLK), NSS: PA28, (NPCS0)
    // REG_PIOA_ABSR |= PIO_ABSR_P25; // Transfer Pin control from PIO to SPI
    // REG_PIOA_PDR |= PIO_PDR_P25;   // Set MISO pin to an output

    // REG_PIOA_ABSR |= PIO_ABSR_P26;    // Transfer Pin control from PIO to SPI
    // REG_PIOA_PDR |= 0 << PIO_PDR_P26; // Set MOSI pin to an input

    // REG_PIOA_ABSR |= PIO_ABSR_P27;    // Transfer Pin control from PIO to SPI
    // REG_PIOA_PDR |= 0 << PIO_PDR_P27; // Set SCLK pin to an input

    // REG_PIOA_ABSR |= PIO_ABSR_P28;    // Transfer Pin control from PIO to SPI
    // REG_PIOA_PDR |= 0 << PIO_PDR_P28; // Set NSS pin to an input

    // REG_SPI0_CR = 1; // Enable SPI
    // REG_SPI0_MR = 0; // Slave mode
    // // Receive Data Register Full Interrupt
    // SPI0->SPI_IER = SPI_IER_RDRF;
    // NVIC_EnableIRQ(SPI0_IRQn);

    // SPI0->SPI_CSR[0] = SPI_CSR_NCPHA | SPI_CSR_BITS_8_BIT; // Shift on falling edge and transfer 8 bits.

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

#endif
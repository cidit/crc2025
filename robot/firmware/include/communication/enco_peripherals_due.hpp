#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "sensors/gobuilda_rotary_enc_data.hpp"
#include "enco_peripherals_common.hpp"

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

// /*
// spi slave
// */

// #define youssef

#ifndef youssef

// #include <ArduinoSTL.h>
#include <Arduino.h>
#include <SPI.h>
#include "communication/enco_peripherals.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "sensors/gobuilda_rotary_enc_data.hpp"

// double dataFrame[16] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.1, 11.2, 12.3, 13.4, 14.5, 15.6, 16.7};
// int byteIndex = 0;  // Current byte to send
// int dataSize = sizeof(dataFrame);  // Total size of the array in bytes
// byte* dataPtr = (byte*)dataFrame;  // Pointer to the array as bytes

bool does_master_need_data()
{
    return digitalRead(SS) == LOW;
}

void spi_slave_crazy_init()
{
    // https://forum.arduino.cc/t/arduino-due-spi-interrupt/574857/4

    // SPI serial recieve
    REG_PMC_PCER0 |= PMC_PCER0_PID24;   // Power up SPI clock
    REG_SPI0_WPMR = 0 << SPI_WPMR_WPEN; // Unlock user interface for SPI

    // Instance SPI0, MISO: PA25, (MISO), MOSI: PA26, (MOSI), SCLK: PA27, (SCLK), NSS: PA28, (NPCS0)
    REG_PIOA_ABSR |= PIO_ABSR_P25; // Transfer Pin control from PIO to SPI
    REG_PIOA_PDR |= PIO_PDR_P25;   // Set MISO pin to an output

    REG_PIOA_ABSR |= PIO_ABSR_P26;    // Transfer Pin control from PIO to SPI
    REG_PIOA_PDR |= 0 << PIO_PDR_P26; // Set MOSI pin to an input

    REG_PIOA_ABSR |= PIO_ABSR_P27;    // Transfer Pin control from PIO to SPI
    REG_PIOA_PDR |= 0 << PIO_PDR_P27; // Set SCLK pin to an input

    REG_PIOA_ABSR |= PIO_ABSR_P28;    // Transfer Pin control from PIO to SPI
    REG_PIOA_PDR |= 0 << PIO_PDR_P28; // Set NSS pin to an input

    REG_SPI0_CR = 1; // Enable SPI
    REG_SPI0_MR = 0; // Slave mode
    // Receive Data Register Full Interrupt
    SPI0->SPI_IER = SPI_IER_RDRF;
    NVIC_EnableIRQ(SPI0_IRQn);

    SPI0->SPI_CSR[0] = SPI_CSR_NCPHA | SPI_CSR_BITS_8_BIT; // Shift on falling edge and transfer 8 bits.
}

void SPI0_Handler()
{
    uint16_t DataReceived;
    uint32_t status = SPI0->SPI_SR;

    if (status & SPI_SR_RDRF)
    {
        DataReceived = SPI0->SPI_RDR & SPI_RDR_RD_Msk;
    }
    // Send something to receive something
    if (status & SPI_SR_TDRE)
    {
        SPI0->SPI_TDR = (uint16_t)DataReceived;
    }
}

const auto ENCO_NUM = 8;
Encoder e[ENCO_NUM] = {
    Encoder(38, 36), // right high
    Encoder(40, 34), // right low
    Encoder(42, 32), // left low
    Encoder(44, 30), // left high
    Encoder(46, 28),
    Encoder(48, 26),
    Encoder(50, 24),
    Encoder(52, 22)};

Timer polling_timer(ONE_SECOND / 4);

void setup()
{
    Serial.begin(115200);
    // spi_slave_crazy_init();
}

void loop()
{
    auto now = millis();
    polling_timer.update(now);
    if (!polling_timer.is_time())
    {
        return;
    }

    for (auto i = 0; i < ENCO_NUM; i++)
    {
        Serial.print(
            "| e" +
            String(i) +
            ": " +
            String(e[i].readAndReset()) +
            " ");
    }
    Serial.println("|");
}

#else

#include <SPI.h>
#include <Encoder.h>

#define CS_SPI_PIN 10

// Encoder EncoA(20, 50);
Encoder EncoB(22, 52);

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

struct mystruct
{
    // uint32_t EncoSwerveR_A;
    uint32_t EncoSwerveR_B;
};

union patate
{
    mystruct spiMsg;
    byte table[sizeof(mystruct) + 1];
};

patate msg;

SPISettings ctrlSPI(1000000, MSBFIRST, SPI_MODE0);

void setup(void)
{
    Serial.begin(115200);
    SPI.begin();
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
    // Serial.print(msg.spiMsg.JoyLeft_X);
    // Serial.println(msg.spiMsg.JoyLeft_Y);
}

void loop(void)
{

    // msg.spiMsg.EncoSwerveR_A = EncoA.read();
    // msg.spiMsg.EncoSwerveR_B = EncoB.read();
    msg.spiMsg.EncoSwerveR_B = 260093299;

    // currentMillis = millis();
    // Serial.println("djjfks");

    // if (runEvery(1000)) {

    for (int i = 0; i < 4; i++)
    {
        // delay(200);
        // delay(1000);
        SPI.beginTransaction(ctrlSPI);
        digitalWrite(10, LOW);
        printf("index : %d ", i);
        printf("   data : %d \n", msg.table[i]);
        byte sentByte = msg.table[i];
        // byte val = 15;
        SPI.transfer(sentByte);
        digitalWrite(10, HIGH);
        SPI.endTransaction();
    }
    // Serial.println(val);
    delay(1000);
    // digitalWrite(CS_SPI_PIN,HIGH);
    // }
}

#endif

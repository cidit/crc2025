// /*
// spi slave
// */

// #define youssef

#ifndef youssef

// #include <ArduinoSTL.h>
#include <Arduino.h>
#include <SPI.h>
#include "util/print.hpp"
#include "communication/enco_peripherals.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "sensors/gobuilda_rotary_enc_data.hpp"

// double dataFrame[16] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.1, 11.2, 12.3, 13.4, 14.5, 15.6, 16.7};
// int byteIndex = 0;  // Current byte to send
// int dataSize = sizeof(dataFrame);  // Total size of the array in bytes
// byte* dataPtr = (byte*)dataFrame;  // Pointer to the array as bytes

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

// dataframe_softcast df;

Timer print_timer(ONE_SECOND);

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

int interupt_c;

void setup()
{
    Serial.begin(115200);
    spi_slave_crazy_init();
}

void loop()
{
    print_timer.update(millis());

    // update dataframe
    auto df_enco_vals = (int32_t *)df;
    for (byte i = 0; i < ENCO_NUM; i++)
    {
        df_enco_vals[i] = e[i].read();
    }

    if (print_timer.is_time())
    {
        SPRINT("c:");
        SPRINT(interupt_c);
        SPRINT(" ");
        for (byte i = 0; i < ENCO_NUM; i++)
        {
            SPRINT("| e");
            SPRINT(i);
            SPRINT(":");
            SPRINT(df_enco_vals[i]);
            SPRINT(" ");
        }
        Serial.println('|');
        auto raw = reinterpret_cast<byte*>(df);
        for (size_t i = 0; i < DATAFRAME_BUFFER_LEN; i++) {
            if (i%4==0) Serial.print(" ");
            if (raw[i] < 10) Serial.print("0");
            Serial.print(raw[i], HEX);
        }
        Serial.println("");
    }
}

void SPI0_Handler()
{
    interupt_c++;
    // uint32_t status = SPI0->SPI_SR;

    // if (status & SPI_SR_RDRF)
    // {

    // MUST READ EVEN IF UNUSED
    // how it would normally be: 
    byte read_value = SPI0->SPI_RDR & SPI_RDR_RD_Msk;
    if (read_value == 0x0F) {
        df_idx = 0;
        // SPI0->SPI_TDR = 0; // send garbage
        // return;
    }
    // (void) (SPI0->SPI_RDR & SPI_RDR_RD_Msk);
    
    //     if (recv == 0x01) {
    //         // 0x01 is what we decided restarts the transmission.
    //         df_idx = 0;
    //     }
    // }

    // Send something to receive something
    // if (status & SPI_SR_TDRE)
    // {
    SPI0->SPI_TDR = (df[df_idx]);
    // }

    if (++df_idx == DF_LEN)
    {
        df_idx = 0;
    }
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

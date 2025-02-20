// /*
// spi slave
// */

#include <Arduino.h>
#include <SPI.h>
#include "util/print.hpp"
#include "communication/enco_peripherals.hpp"
#include "sensors/gobuilda_rotary_enc.hpp"
#include "sensors/gobuilda_rotary_enc_data.hpp"

Timer print_timer(ONE_SECOND);

Encoder e[ENCO_NUM] = {
    Encoder(38, 36), // right high
    Encoder(40, 34), // right low (ITS INVERTED!)
    Encoder(42, 32), // left low
    Encoder(44, 30), // left high
    Encoder(46, 28),
    Encoder(48, 26),
    Encoder(50, 24),
    Encoder(52, 22),
};

dataframe_t df;

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

    // reset the idx if slave is deselected
    if (df_idx != 0 && digitalRead(SS) == HIGH)
    {
        df_idx = 0;
    }

    // update dataframe
    for (byte i = 0; i < ENCO_NUM; i++)
    {
        df[i] = e[i].read();
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
            SPRINT(df[i]);
            SPRINT(" ");
        }
        Serial.println('|');
        auto raw = reinterpret_cast<byte *>(df);
        for (size_t i = 0; i < DF_LEN; i++)
        {
            if (i % 4 == 0)
                Serial.print(" ");
            if (raw[i] < 10)
                Serial.print("0");
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
    if (read_value == 0x0F)
    {
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
    auto raw = reinterpret_cast<byte *>(df);
    SPI0->SPI_TDR = (raw[df_idx]);
    // }

    if (++df_idx == DF_LEN)
    {
        df_idx = 0;
    }
}

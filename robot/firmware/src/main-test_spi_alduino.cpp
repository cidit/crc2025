/*
spi slave
*/

// #include <ArduinoSTL.h>
#include <Arduino.h>
#include <SPI.h>
#include "communication/enco_peripherals.hpp"

// double dataFrame[16] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.1, 11.2, 12.3, 13.4, 14.5, 15.6, 16.7};
// int byteIndex = 0;  // Current byte to send
// int dataSize = sizeof(dataFrame);  // Total size of the array in bytes
// byte* dataPtr = (byte*)dataFrame;  // Pointer to the array as bytes

bool does_master_need_data()
{
    return digitalRead(SS) == LOW;
}

void setup()
{
    Serial.begin(115200);

    pinMode(SS, INPUT);
    SPI.begin();

    SPI.setClockDivider(SPI_CLOCK_DIV8); // ajust to match clock speed with master
}

void loop()
{

    if (does_master_need_data())
    {
        Serial.println("answering master...");
        dataframe_softcast df;
        df.data = {{{1.1, 2.1}, {3.1, 4.1}, {5.1, 6.1}, {9.1, 10.1}, {11.1, 12.1}, {13.1, 14.1}, {15.1, 16.1}}};
        SPI.beginTransaction(SPI_AL_CRC_SETTINGS);
        for (auto rawb : df.bytes)
        {
            auto recv = SPI.transfer(rawb);
            Serial.print(recv);
            Serial.print("|");
        }
        Serial.println();
        SPI.endTransaction();
    }
}

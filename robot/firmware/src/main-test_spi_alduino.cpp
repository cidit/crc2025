// /*
// spi slave
// */

// // #include <ArduinoSTL.h>
// #include <Arduino.h>
// #include <SPI.h>
// #include "communication/enco_peripherals.hpp"

// // double dataFrame[16] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.1, 11.2, 12.3, 13.4, 14.5, 15.6, 16.7};
// // int byteIndex = 0;  // Current byte to send
// // int dataSize = sizeof(dataFrame);  // Total size of the array in bytes
// // byte* dataPtr = (byte*)dataFrame;  // Pointer to the array as bytes

// bool does_master_need_data()
// {
//     return digitalRead(SS) == LOW;
// }

// void setup()
// {
//     Serial.begin(115200);

//     pinMode(SS, INPUT);
//     SPI.begin();

//     SPI.setClockDivider(SPI_CLOCK_DIV8); // ajust to match clock speed with master
// }

// void loop()
// {

//     if (does_master_need_data())
//     {
//         Serial.println("answering master...");
//         dataframe_softcast df;
//         df.data = {{{1.1, 2.1}, {3.1, 4.1}, {5.1, 6.1}, {9.1, 10.1}, {11.1, 12.1}, {13.1, 14.1}, {15.1, 16.1}}};
//         SPI.beginTransaction(SPI_AL_CRC_SETTINGS);
//         for (auto rawb : df.bytes)
//         {
//             auto recv = SPI.transfer(rawb);
//             Serial.print(recv);
//             Serial.print("|");
//         }
//         Serial.println();
//         SPI.endTransaction();
//     }
// }


#include <SPI.h>
#include <Encoder.h>


#define CS_SPI_PIN 10

// Encoder EncoA(20, 50);
Encoder EncoB(22, 52);

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;



typedef struct mystruct {
  // uint32_t EncoSwerveR_A;
  uint32_t EncoSwerveR_B;
};

typedef union patate {
  mystruct spiMsg;
  byte table[sizeof(mystruct) + 1];
};

patate msg;


SPISettings ctrlSPI(1000000, MSBFIRST, SPI_MODE0);

boolean runEvery(unsigned long interval) {
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}


void setup(void) {
  Serial.begin(115200);
  SPI.begin();
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  // Serial.print(msg.spiMsg.JoyLeft_X);
  // Serial.println(msg.spiMsg.JoyLeft_Y);
}

void loop(void) {

  // msg.spiMsg.EncoSwerveR_A = EncoA.read();
  // msg.spiMsg.EncoSwerveR_B = EncoB.read();
  msg.spiMsg.EncoSwerveR_B = 260093299;

  // currentMillis = millis();
  // Serial.println("djjfks");

  // if (runEvery(1000)) {

  for (int i = 0; i < 4; i++) {
    //delay(200);
    //delay(1000);
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

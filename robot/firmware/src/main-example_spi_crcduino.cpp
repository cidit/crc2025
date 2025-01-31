// /*
// spi master
// */

// #define youssef

#ifndef youssef

#include <Arduino.h>
#include <SPI.h>
#include "communication/enco_peripherals.hpp"
#include "util/constants.hpp"
#include "util/timer.hpp"
#include "util/print.hpp"


Timer poll_timer(ONE_SECOND/1);

void setup() {
    Serial.begin(115200);
    master_enco_spi_init();
    SPI.begin();
}

void loop() {
    poll_timer.update(millis());

    if (poll_timer.is_time()) {
        int32_t df[ENCO_NUM];
        // TODO: for some reason, the values seem to be multiplied by 256
        retrieve_df(df);
        for (auto i = 0; i < ENCO_NUM; i++) {
            SPRINT("| e");
            SPRINT(i);
            SPRINT(":");
            SPRINT(df[i]);
            SPRINT(" ");
        }
        Serial.println("|");
        auto raw = reinterpret_cast<byte*>(df);
        for (size_t i = 0; i < DATAFRAME_BUFFER_LEN; i++) {
            if (i%4==0) Serial.print(" ");
            if (raw[i] < 10) Serial.print("0");
            Serial.print(raw[i], HEX);
        }
        Serial.println("");
    }
    
}

#else

#include <SPI.h>
#define CS_SPI_PIN 53

struct mystruct {
  // uint32_t EncoSwerveR_A;
  uint32_t EncoSwerveR_B;
};

union patate {
  mystruct spiMsg;
  byte table[sizeof(mystruct)];
};
patate msg;

volatile bool received = false;

// float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
//   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }


void setup() {
  Serial.begin(115200);
  pinMode(CS_SPI_PIN, OUTPUT);
  // pinMode(MISO, INPUT);  //Configure MISO en sortie (pour envoyer des données au contrôleur)
  // pinMode(MOSI, OUTPUT);
  digitalWrite(CS_SPI_PIN, HIGH);
  // SPCR |= _BV(MSTR);      //Configure l'interface SPI comme étant un périphérique
  SPI.attachInterrupt();  //Permet les interruptions sur la réception de données SPI
}

ISR(SPI_STC_vect) {
  static byte index = 0;
  byte receivedByte = SPDR;
  msg.table[index++] = receivedByte;
  if (index == sizeof(mystruct)) {
    index = 0;
    received = true;
  }
}


void loop() {
  if (received) {
    Serial.println(msg.spiMsg.EncoSwerveR_B);
    received = false;
  }
}

#endif
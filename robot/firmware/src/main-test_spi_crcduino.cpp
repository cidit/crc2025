// /*
// spi master
// */


// #include <Arduino.h>
// #include <SPI.h>
// #include "communication/enco_peripherals.hpp"
// #include "util/constants.hpp"
// #include "util/timer.hpp"
// #include "util/print.hpp"

// Timer spi_poll_timer(ONE_SECOND/1);

// void myinit() {
//     pinMode(SS, OUTPUT);
//     digitalWrite(SS, HIGH);
// }

// dataframe retrieve_df() {
//     dataframe_softcast dfs;

//     SPI.beginTransaction(SPI_AL_CRC_SETTINGS);
//     digitalWrite(SS, LOW); // INITIATE COMM WITH SLAVE
//     for (uint8_t i = 0; i < sizeof(dfs.bytes); i++) {
//         dfs.bytes[i] = SPI.transfer(i);
//     }
//     digitalWrite(SS, HIGH); // END COMM
//     SPI.endTransaction();

//     return dfs.data;
// }

// void setup() {
//     Serial.begin(115200);
    
//     myinit();
//     SPI.begin();
//     // SPI.begin(SS);
//     // SPI.setDataMode(SS, SPI_MODE0);
//     // SPI.setClockDivider(SS, SPI_CLOCK_DIV8); // ajust to match clock speed with master
// }

// void loop() {
//     spi_poll_timer.update(millis());

//     if (spi_poll_timer.is_time()) {
//         auto df = retrieve_df();
//         for (auto data: df.data) {
//             SPRINT("v:" + String(data.rpm));
//             SPRINT("a:"+ String(data.rads));
//             SEPARATOR;
//         }
//         SPRINT('\n');
//     }
    
// }



#include <SPI.h>
#define CS_SPI_PIN 53

typedef struct mystruct {
  // uint32_t EncoSwerveR_A;
  uint32_t EncoSwerveR_B;
};

typedef union patate {
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

#include <SPI.h>
#include <Servo.h>
#define CS_SPI_PIN 10

int pins[] = { 9, 6 };
const int arraySize = sizeof(pins) / sizeof(int);
Servo myservo[arraySize];

typedef struct mystruct {
  uint8_t JoyLeft_X;
  uint8_t JoyLeft_Y;
  // uint8_t JoyRight_X;
  // uint8_t JoyRight_Y;
  // bool boutton_A;
  // bool boutton_B;
  // bool boutton_X;
  // bool boutton_Y;
  // bool boutton_UP;
  // bool boutton_DOWN;
  // bool boutton_RIGHT;
  // bool boutton_LEFT;
};

typedef union patate {
  mystruct spiMsg;
  byte table[sizeof(mystruct)];
};

patate msg;

volatile bool received = false;

void setup() {
  Serial.begin(115200);
  pinMode(CS_SPI_PIN, OUTPUT);
  pinMode(MISO, OUTPUT);  //Configure MISO en sortie (pour envoyer des données au contrôleur)
  digitalWrite(CS_SPI_PIN, HIGH);
  SPCR |= _BV(SPE);       //Configure l'interface SPI comme étant un périphérique
  SPI.attachInterrupt();  //Permet les interruptions sur la réception de données SPI
  for (int i = 0; i < 2; i++)
    myservo[i].attach(pins[i]);
  pinMode(pins, OUTPUT);
}

ISR(SPI_STC_vect) {
  static byte index = 0;
  byte receivedByte = SPDR;
  msg.table[index++] = receivedByte;
  //Serial.print("Received byte: ");
  //Serial.println(receivedByte, HEX);
  if (index == sizeof(mystruct)) {
    index = 0;
    received = true;
  }
}


void loop() {
  if (received) {
    Serial.print("JoyLeft_X: ");
    Serial.println(msg.spiMsg.JoyLeft_X);
    Serial.print("JoyLeft_Y: ");
    Serial.println(msg.spiMsg.JoyLeft_Y);
    received = false;
  }
  int joyL_X = map(msg.spiMsg.JoyLeft_X,0,255,-100,100);
  int joyL_Y = map(msg.spiMsg.JoyLeft_Y,0,255,-100,100);
  int PWMvalueY = joyL_Y * 5 + 1500;
  int PWMvalueX = joyL_X * 5 + 1500;
  myservo[0].writeMicroseconds(PWMvalueX);
  myservo[1].writeMicroseconds(PWMvalueY);
  // myservo[2].writeMicroseconds(0);
  //myservo[1].writeMicroseconds(0);
}

#include <SPI.h>
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

XboxSeriesXControllerESP32_asukiaaa::Core xboxController;

const uint16_t CS_SPI_PIN = 5;

typedef struct joystick_t {
  uint8_t x, y;
}

typedef struct controller_t
{
  joystick_t a, b;
};
typedef union ctl_bytes_u {
  controller_t ctl;
  
}

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

volatile boolean flagRxData = false;  //Flag pour indiquer qu'une donnée a été reçue sur l'interface SPI
volatile byte periphTxData, periphRxData;

SPISettings ctrlSPI(1000000, MSBFIRST, SPI_MODE0);


void setup(void) {
  Serial.begin(115200);
  pinMode(CS_SPI_PIN, OUTPUT);
  SPI.begin();
  xboxController.begin();
  Serial.print(msg.spiMsg.JoyLeft_X);
  Serial.println(msg.spiMsg.JoyLeft_Y);
}

void loop(void) {
  xboxController.onLoop();
  if (xboxController.isConnected()) {
    if (xboxController.isWaitingForFirstNotification()) {
      Serial.println("waiting for first notification");
    } else {
      uint8_t joy1_Y = map(xboxController.xboxNotif.joyLVert, 0, 65535, 0, 255);
      uint8_t joy1_X = map(xboxController.xboxNotif.joyLHori, 0, 65535, 0, 255);
      //double zbi = rads(joy1_X, joy1_Y);
      msg.spiMsg.JoyLeft_X = joy1_X;
      msg.spiMsg.JoyLeft_Y = joy1_Y;

      //Serial.println(String(zbi) + "::" + String(joy1_X) + "::" + String(joy1_Y));
    }
  } else {
    Serial.println("not connected");
    if (xboxController.getCountFailedConnection() > 2) {
      ESP.restart();
    }

    //i = 0;
    //Serial.print("Sent byte: ");
    //Serial.println(sentByte, HEX);
  }
  SPI.beginTransaction(ctrlSPI);
  for (int i = sizeof(msg.table) - 1; i >= 0; i--) {
    Serial.println(msg.table[i]);
    byte sentByte = msg.table[i];
    SPI.transfer(sentByte);
    //delay(200);
    SPI.endTransaction();
    //delay(1000);
  }
}

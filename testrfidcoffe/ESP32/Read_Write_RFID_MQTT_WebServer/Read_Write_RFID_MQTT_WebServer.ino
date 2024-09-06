#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);  // Create instance of the MFRC522 class

MFRC522::MIFARE_Key key;

// Array to store the UID of the card
byte nuidPICC[4];

WiFiClient espClient;
PubSubClient client(espClient);

// WiFi credentials
const char* ssid = "";
const char* password = "";

// MQTT broker details
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;      // Standard MQTT port
const char* mqttUser = "";      // Username (if required)
const char* mqttPassword = "";  // Password (if required)

// MQTT topics
const char* subscribeTopic = "rfid";
const char* publishTopic = "rfidondro";

// Timing variables
int previousMillis = 0;
const long interval = 10000;  // Interval to perform tasks (10 seconds)
const int keepAliveInterval = 60;  // MQTT keep-alive interval (in seconds)

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  SPI.begin();      // Initialize SPI bus
  rfid.PCD_Init();  // Initialize MFRC522 RFID reader

  // Set default key for RFID authentication
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scans the MIFARE Classic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  // Set MQTT server and callback
  client.setServer(mqttServer, mqttPort);
  client.setKeepAlive(keepAliveInterval);
  client.setCallback(callback);
}

// Function to connect to MQTT broker
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
      client.subscribe(subscribeTopic);  // Subscribe to a topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Function to reconnect to MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(subscribeTopic);  // Subscribe to a topic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// MQTT callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Function to detect and handle new RFID cards
void detectNewCard() {
  // Check for new card presence
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check if the card UID is different from the previous one
  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
      rfid.uid.uidByte[1] != nuidPICC[1] || 
      rfid.uid.uidByte[2] != nuidPICC[2] || 
      rfid.uid.uidByte[3] != nuidPICC[3]) {

    Serial.println(F("A new card has been detected."));

    // Store the new UID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    // Construct the message to be sent
    String uidStr = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uidStr += String(rfid.uid.uidByte[i], HEX);
    }
    String payload = "UID=" + uidStr;

    // Publish the message to the MQTT broker
    if (client.publish(publishTopic, payload.c_str())) {
      Serial.println("Data published: " + payload);
    } else {
      Serial.println("Failed to publish data");
    }
  } else {
    Serial.println(F("Card read previously."));
  }

  // Halt PICC (Power down the card)
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void loop() {
  if (!client.connected()) {
    reconnect();  // Reconnect to MQTT if disconnected
  }
  client.loop();

  detectNewCard();  // Detect and handle new RFID cards
}

// Helper function to print bytes in hex format
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

// Helper function to print bytes in decimal format
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i]);
  }
}

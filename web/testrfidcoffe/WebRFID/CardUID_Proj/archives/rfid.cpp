#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Jahm";
const char* password = "JahmTheBeast_445";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 8000;
const char* mqttTopic = "rfidondro";

WiFiClient espClient;
PubSubClient client(espClient);

String UID = "7F7S9A0F9";
String Name = "ondro";

unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  client.setServer(mqttServer, mqttPort);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      // Successfully connected
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    String postData = "CardUID= " + UID + " &Name= " + Name;
    client.publish(mqttTopic, postData.c_str());

    Serial.println("Data published to MQTT: " + postData);
  }
}

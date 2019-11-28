#include <WiFi.h>
#include <PubSubClient.h>

#include "esp_wifi.h"
#include "soc/sens_reg.h"

/* change it with your ssid-password */
#define SSID        "IoTnet"
#define PASSWORD    "darksecret"

#define MQTT_SERVER "192.168.8.229"
#define MQTT_TOPIC  "esi/room1/temp"
#define MQTT_CLIENT_ID "ESP32Client"

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

char msg[20];

uint64_t read_ctl2;

void wifi_connect() {
  // https://github.com/espressif/arduino-esp32/issues/102
  // https://www.gitmemory.com/issue/espressif/arduino-esp32/102/496284224
  read_ctl2 = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);

  Serial.println();
  Serial.print("WiFi connecting to ");
  Serial.print(SSID);

  esp_wifi_start();
  WiFi.begin(SSID, PASSWORD);
  delay(500);

  while (!WiFi.isConnected()) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("-> connected");
  Serial.print("node IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_connect() {
  while (!client.connected()) {
    Serial.print("MQTT connecting: ");

    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("-> connected");
    } else {
      Serial.print("failed, status code = ");
      Serial.print(client.state());
      Serial.println(", try again in 5 seconds");
      delay(5000);
    }
  }
}

int read_sensor() {
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, read_ctl2);
  int value = analogRead(A0);
  return value;
}

void send_reading(int reading) {
  if (!client.connected()) {
    mqtt_connect();
  }

  snprintf(msg, 20, "%d", reading);
  client.publish(MQTT_TOPIC, msg);
  Serial.print("temp published: ");
  Serial.println(reading);
}

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  wifi_connect();
}

void loop() {
  int reading = read_sensor();
  client.setServer(MQTT_SERVER, 1883);
  send_reading(reading);
  delay(3000);
}
// -*- coding: utf-8; mode: c++; tab-width: 4 -*-

#include <WiFi.h>
#include <PubSubClient.h>

#include <esp_int_wdt.h>
#include <esp_task_wdt.h>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "soc/soc.h"
#include "soc/sens_reg.h"

/* change it with your ssid-password */
#define SSID        "IoTnet"
#define PASSWORD    "darksecret"
#define MQTT_SERVER "192.168.8.229"

int reading = 0;

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

#define TOPIC "esi/room1/temp"
char msg[20];

uint64_t reg_a;
uint64_t reg_b;
uint64_t reg_c;

void wifi_connect() {
  reg_a = READ_PERI_REG(SENS_SAR_START_FORCE_REG);
  reg_b = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);
  reg_c = READ_PERI_REG(SENS_SAR_MEAS_START2_REG);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("node IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_connect() {
  while (!client.connected()) {
    Serial.print("MQTT connecting... ");
    String clientId = "ESP32Client";

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");

      delay(5000);  //* Wait 5 seconds before retrying
    }
  }
}

float read_sensor() {
  adcAttachPin(A0);
  int value = analogRead(A0);
//  Serial.println(value);
  return value;
}

void send_reading() {
  if (!client.connected()) {
    mqtt_connect();
  }

  snprintf(msg, 20, "%d", reading);
  client.publish(TOPIC, msg);
  Serial.print("temp published: ");
  Serial.println(reading);
}

void wifi_disconnect() {
// https://github.com/espressif/arduino-esp32/issues/102

//  WiFi.mode(WIFI_MODE_NULL);
//  WiFi.disconnect();
//  WiFi.mode(WIFI_MODE_NULL);

  esp_wifi_stop();
  esp_wifi_deinit();
//  nvs_flash_deinit();

  WRITE_PERI_REG(SENS_SAR_START_FORCE_REG, reg_a);  // fix ADC registers
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, reg_b);
  WRITE_PERI_REG(SENS_SAR_MEAS_START2_REG, reg_c);
}


void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  reading = read_sensor();
  wifi_connect();
  client.setServer(MQTT_SERVER, 1883);
  send_reading();
  wifi_disconnect();
  delay(5000);
  ESP.restart();
}

void loop() {
}

#include <Arduino.h>
#include "WiFi.h"
#include <analogWrite.h>
#include "ping.h"

const char* ssid = "IoTnet";
const char* password = "darksecret";

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }

  Serial.println("Connected to the WiFi network");
}

void loop() {
  Serial.print("router: ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  delay(5000);
}



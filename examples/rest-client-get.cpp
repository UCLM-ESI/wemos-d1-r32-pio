#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "IoTnet";
const char* password =  "darksecret";
 
void setup() {
  Serial.begin(9600);
  delay(4000);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
}
 
void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
 
    HTTPClient http;
 
    http.begin("http://192.168.8.229:8000/hola.txt");
    int httpCode = http.GET();
 
    if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end();
  }
 
  delay(10000);
}
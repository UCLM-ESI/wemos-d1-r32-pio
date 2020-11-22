// Levantar server HTTP en Host con fichero hello.txt 
// python -m SimpleHTTPServer 8000


#include <WiFi.h>
#include <HTTPClient.h>
 
const char* ssid = "IoTnet";
const char* password = "darksecret";
 
 
void setup() {
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
 
    HTTPClient http;
 
    http.begin("http://192.168.43.231:8000/hello.txt");
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

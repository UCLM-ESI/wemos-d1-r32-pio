#include <WiFi.h>
#include <ESPAsyncWebServer.h>
 

AsyncWebServer server(80);
const char* ssid = "IoTnet";
const char* password = "darksecret";
 

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void routeHello(AsyncWebServerRequest *request)
{
  request->send(200, "application/json", "{\"message\":\"hello\"}");
}

void wifiConnect()
{
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(115200);
  delay(4000);
  wifiConnect();

  server.on("/hello", HTTP_GET, routeHello);
  server.onNotFound(notFound);
  server.begin();
}
 
void loop() {}

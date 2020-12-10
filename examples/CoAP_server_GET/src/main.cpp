#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
#include <coap_server.h>
 
const char* ssid = "lanHome10";//"IoTnet";
const char* password = "@lanhome10";//"darksecret";


// UDP and CoAP class
WiFiUDP udp;
Coap coap(udp);
//coapServer coap;


// CoAP server endpoint URL
void callback_hello(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("Hello CoAP");
  coap.sendResponse(ip, port, packet.messageid, "Hello from ESP32",16,
                    COAP_VALID,COAP_TEXT_PLAIN,packet.token,packet.tokenlen);
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
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(4000);
  wifiConnect();

  coap.server(callback_hello, "hello");
  coap.start(5683);
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
  coap.loop();
}
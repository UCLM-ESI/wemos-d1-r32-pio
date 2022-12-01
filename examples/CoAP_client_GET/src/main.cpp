#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>
 
const char* ssid = "IoTnet";
const char* password = "darksecret";


// UDP and CoAP class
WiFiUDP udp;
Coap coap(udp);
//coapServer coap;

void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.printf("[CoAP Response]\r\n");
  
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;
  
  Serial.println(p);
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

  coap.response(callback_response);
  coap.start(5683);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  uint16_t packet_id = coap.get(IPAddress(192,168,3,106), 5683, "hello");
  printf("Packet ID: %d\r\n", packet_id);
  delay(500);
  coap.loop();

  delay(8000);
}

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "dhtMessage.pb.h"

#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"

const char* ssid = "IoTnet";
const char* password =  "darksecret";

#define TOPIC "esi/master/jcj"
#define BROKER_IP "192.168.1.103"
#define BROKER_PORT 2883

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(D4, DHT11);

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

void mqttConnect() {
  client.setServer(BROKER_IP, BROKER_PORT);
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");

    if (client.connect("ESP32Client1")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");

      delay(5000);  //* Wait 5 seconds before retrying
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

  delay(4000);
  wifiConnect();
  mqttConnect();
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(3000);

  uint8_t buffer[200];

  dhtMessage message = dhtMessage_init_zero;

  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

  message.t = dht.readTemperature();
  message.has_t = true;
  message.h = dht.readHumidity();
  message.has_h = true;
  //message.f = 0.5;
  //strcpy(message.message, "Hello Protobuf!");
 // message.op = 0x48656c6c;

  bool status = pb_encode(&stream, dhtMessage_fields, &message);

  if (!status)
  {
      Serial.println("Failed to encode");
      return;
  }
  
  client.publish(TOPIC, buffer, stream.bytes_written);
}

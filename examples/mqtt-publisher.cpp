#include <WiFi.h>
#include <PubSubClient.h>

/* change it with your ssid-password */
const char* ssid = "IoTnet";
const char* password = "darksecret";

const char* mqtt_server = "192.168.8.229";
float temperature = 0;

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

#define TOPIC "esi/room1/temp"

long last = 0;
char msg[20];

void mqttconnect() {
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
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

void setup() {
  Serial.begin(9600);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("node IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    mqttconnect();
  }

  long now = millis();
  if (now - last > 3000) {
    last = now;
    temperature = 20.3;
    snprintf(msg, 20, "%lf", temperature);
    client.publish(TOPIC, msg);
    Serial.println("temp published");
  }
}
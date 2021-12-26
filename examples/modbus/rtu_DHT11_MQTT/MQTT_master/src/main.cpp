#include <Arduino.h>
#include <ModbusRTU.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define NO_VERBOSE

const char* ssid = "IoTnet";
const char* password =  "darksecret";

#define TOPIC "esi/lab/arco"
#define BROKER_IP "192.168.0.101"
#define BROKER_PORT 2883

const uint8_t REG_TEMPERATURE = 10;
const uint8_t REG_HUMIDITY = 30;
const uint8_t SLAVE_ID = 1;


typedef union _CAST_IEEE754{
  float f;
  uint16_t i16[2];
} cast_ieee754_t;


ModbusRTU mb;
WiFiClient espClient;
PubSubClient client(espClient);

static uint16_t coilsTemperature[2];
static uint16_t coilsHumidity[2];


QueueHandle_t xQueueTemperature;
QueueHandle_t xQueueHumidity;


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

    if (client.connect("RTUMasterClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");

      delay(5000);  //* Wait 5 seconds before retrying
    }
  }
}


bool cbReadTemperature(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  portBASE_TYPE xStatus;  
  cast_ieee754_t _temperature;
  _temperature.i16[0] = coilsTemperature[0];
  _temperature.i16[1] = coilsTemperature[1];
  const portTickType xTicksToWait = 500 / portTICK_RATE_MS;

#ifdef VERBOSE
  printf("Temperature: %f (%d  :  %d)\r\n", _temperature.f, coilsTemperature[0], coilsTemperature[1]);
#endif

  xStatus = xQueueSendToBack( xQueueTemperature, &_temperature.f, xTicksToWait );
#ifdef VERBOSE
  if( xStatus != pdPASS )
    printf("[TEMPERATURE] Could not send to the queue.\r\n");
#endif

  return true;
}


bool cbReadHumidity(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  portBASE_TYPE xStatus;  
  cast_ieee754_t _humidity;
  _humidity.i16[0] = coilsHumidity[0];
  _humidity.i16[1] = coilsHumidity[1];
  const portTickType xTicksToWait = 500 / portTICK_RATE_MS;

#ifdef VERBOSE
  printf("Humidity: %f (%d  :  %d)\r\n", _humidity.f, coilsHumidity[0], coilsHumidity[1]);
#endif

  xStatus = xQueueSendToBack( xQueueHumidity, &_humidity.f, xTicksToWait );
#ifdef VERBOSE
  if( xStatus != pdPASS )
    printf("[HUMIDITY] Could not send to the queue.\r\n");
#endif
  
  return true;
}


static void vTaskTemperature( void *pvParameters )
{ 
  for( ;; ){    
    mb.readHreg(SLAVE_ID, REG_TEMPERATURE, &coilsTemperature[0], 2, cbReadTemperature);
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


static void vTaskHumidity( void *pvParameters )
{ 
  for( ;; ){    
    mb.readHreg(SLAVE_ID, REG_HUMIDITY, &coilsHumidity[0], 2, cbReadHumidity);
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


static void vTaskPublish( void *pvParameters )
{ 
  float temperature;
  float humidity;
  const portTickType xTicksToWait = 10000 / portTICK_RATE_MS;

  for( ;; ){  
    xQueueReceive( xQueueTemperature, &temperature, xTicksToWait );
    xQueueReceive( xQueueHumidity, &humidity, xTicksToWait );

    String jsonData = "{\"temperature\":"+String(temperature)+",\"humidity\":"+String(humidity)+"}";
    client.publish(TOPIC, jsonData.c_str());
    
    vTaskDelay(5000/portTICK_PERIOD_MS); 
  }
  vTaskDelete(NULL);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  wifiConnect();
  mqttConnect();

  delay(2000);

  mb.begin(&Serial);

  xQueueTemperature = xQueueCreate( 5, sizeof( float ) );
  xQueueHumidity = xQueueCreate( 5, sizeof( float ) );
  if( xQueueTemperature != NULL || xQueueHumidity != NULL )
  {
    xTaskCreatePinnedToCore( vTaskTemperature, "Temperature", 4096, NULL, 3, NULL, 1 );
    xTaskCreatePinnedToCore( vTaskHumidity, "Humidity", 4096, NULL, 3, NULL, 1 );
    xTaskCreatePinnedToCore( vTaskPublish, "MQTT_Publisher", 4096, NULL, 1, NULL, 1 );
    mb.master();
  }
}


void loop() {
  mb.task();
  delay(10);
  yield();
}

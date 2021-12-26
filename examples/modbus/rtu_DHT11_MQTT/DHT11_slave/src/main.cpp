#include <Arduino.h>
#include <ModbusRTU.h>
#include <DHT.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define VERBOSE

const uint8_t REG_TEMPERATURE_HIGH = 10;
const uint8_t REG_TEMPERATURE_LOW = 11;
const uint8_t REG_HUMIDITY_HIGH = 30;
const uint8_t REG_HUMIDITY_LOW = 31;
const uint8_t SLAVE_ID = 1;


typedef union _CAST_IEEE754{
  float f;
  uint16_t i16[2];
} cast_ieee754_t;


ModbusRTU mb;
DHT dht(D4, DHT11);


static void vTaskTemperature( void *pvParameters )
{ 
  cast_ieee754_t _temperature;

  for( ;; ){    
    float value = dht.readTemperature();
    _temperature.f = value;
    
#ifdef VERBOSE
  printf("Temperature: %f\r\n", value);
#endif

    mb.Hreg(REG_TEMPERATURE_HIGH, _temperature.i16[0]);
    mb.Hreg(REG_TEMPERATURE_LOW, _temperature.i16[1]);
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


static void vTaskHumidity( void *pvParameters )
{ 
  cast_ieee754_t _humidity;

  for( ;; ){    
    float value = dht.readHumidity();
    _humidity.f = value;

#ifdef VERBOSE
  printf("Humidity: %f\r\n", value);
#endif

    mb.Hreg(REG_HUMIDITY_HIGH, _humidity.i16[0]);
    mb.Hreg(REG_HUMIDITY_LOW, _humidity.i16[1]);
    vTaskDelay(10000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mb.begin(&Serial);
  dht.begin();

  mb.slave(SLAVE_ID);
  mb.addHreg(REG_TEMPERATURE_HIGH);
  mb.Hreg(REG_TEMPERATURE_HIGH, 0);
  mb.addHreg(REG_TEMPERATURE_LOW);
  mb.Hreg(REG_TEMPERATURE_LOW, 0);
  mb.addHreg(REG_HUMIDITY_HIGH);
  mb.Hreg(REG_HUMIDITY_HIGH, 0);
  mb.addHreg(REG_HUMIDITY_LOW);
  mb.Hreg(REG_HUMIDITY_LOW, 0);

  delay(4000);

  xTaskCreatePinnedToCore( vTaskTemperature, "Temperature", 4000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore( vTaskHumidity, "Humidity", 4000, NULL, 1, NULL, 1);
}


void loop() {
  // put your main code here, to run repeatedly:
  mb.task();
  delay(200);
  yield();
}

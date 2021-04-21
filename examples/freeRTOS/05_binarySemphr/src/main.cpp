#include <Arduino.h>

#include "nvs_flash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"


QueueHandle_t xBinarySemaphore;

void IRAM_ATTR sw1_handleInterrupt(){
  static portBASE_TYPE xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
  xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
}


static void vPeriodicTask( void *pvParameters )
{
  for( ;; ){
    vTaskDelay( 500 / portTICK_RATE_MS );
    printf( "Periodic task is running.\r\n" );
  }
  vTaskDelete(NULL);
}

static void vHandlerTask( void *pvParameters )
{
  for( ;; ){
    xSemaphoreTake( xBinarySemaphore, portMAX_DELAY );
    printf( "Handler task - Processing event.\r\n" );
  }
  vTaskDelete(NULL);
}


void app_main(void)
{
  vSemaphoreCreateBinary( xBinarySemaphore );
  
  if( xBinarySemaphore != NULL )
  {
    xTaskCreate( vHandlerTask, "Handler", 1000, NULL, 3, NULL );
    xTaskCreate( vPeriodicTask, "Periodic", 1000, NULL, 1, NULL );
  }
}

void setup() {
  // put your setup code here, to run once:

  pinMode(D2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D2), sw1_handleInterrupt, FALLING);

  app_main();
}

void loop() {
  // put your main code here, to run repeatedly:
}


#include <Arduino.h>

#include "nvs_flash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
//#include "freertos/heap_regions.h"


#define CONFIG_ESP32_PANIC_GDBSTUB 1


QueueHandle_t xQueue;


void vSenderTask(void* pvParam)
{
  int valueToSend;
  portBASE_TYPE xStatus;  
  valueToSend = ( int ) pvParam;
  const portTickType xTicksToWait = 500 / portTICK_RATE_MS;

  for(;;) {
    xStatus = xQueueSendToBack( xQueue, &valueToSend, xTicksToWait );
    if( xStatus != pdPASS )
      printf("Could not send to the queue.\r\n");
    vTaskDelay(500/portTICK_RATE_MS);
  }
  vTaskDelete(NULL);
}


void vReceiverTask(void* pvParam)
{
  int receivedValue;
  portBASE_TYPE xStatus;
  const portTickType xTicksToWait = 10000 / portTICK_RATE_MS;

  for(;;) {
    //if( uxQueueMessagesWaiting( xQueue ) != 0 )
    //  printf( "Queue should have been empty!\r\n" );

    //while( uxQueueMessagesWaiting( xQueue ) != 5 );
    //printf( "Queue should have been full (%d/5)!\r\n",  uxQueueMessagesWaiting( xQueue ));

    xStatus = xQueueReceive( xQueue, &receivedValue, xTicksToWait );
    if( xStatus == pdPASS ){
      printf( "Received = %d\n", receivedValue);
    }
    else
      printf( "Could not receive from the queue.\r\n" );
  }
  vTaskDelete(NULL);
}


void app_main(void)
{
  nvs_flash_init();

  xQueue = xQueueCreate( 5, sizeof( int ) );
  if( xQueue != NULL )
  {
    xTaskCreate( vReceiverTask, "Receiver", 4096, NULL, 5, NULL );
    xTaskCreate( vSenderTask, "Sender1", 4096, ( void * ) 100, 1, NULL );
    xTaskCreate( vSenderTask, "Sender2", 4096, ( void * ) 200, 1, NULL );
  }
}

void setup() {
  // put your setup code here, to run once:
  app_main();
}

void loop() {
  // put your main code here, to run repeatedly:
}


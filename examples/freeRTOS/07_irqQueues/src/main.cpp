#include <Arduino.h>

#include "nvs_flash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"


QueueHandle_t xIntegerQueue;
QueueHandle_t xStringQueue;

void IRAM_ATTR sw1_handleInterrupt(){
  static portBASE_TYPE xHigherPriorityTaskWoken;
  static unsigned long ulReceivedNumber;
  static const char *pcStrings[] =
  {
    "String 0\n",
    "String 1\n",
    "String 2\n",
    "String 3\n"
  };
  xHigherPriorityTaskWoken = pdFALSE;
  /* Loop until the queue is empty. */
  while( xQueueReceiveFromISR( xIntegerQueue, &ulReceivedNumber, &xHigherPriorityTaskWoken ) != errQUEUE_EMPTY )
  {
    ulReceivedNumber &= 0x03;
    xQueueSendToBackFromISR( xStringQueue, &pcStrings[ ulReceivedNumber ], &xHigherPriorityTaskWoken );
  }
}


static void vStringPrinter( void *pvParameters )
{
  char *pcString;
  for( ;; ){
    /* Block on the queue to wait for data to arrive. */
    xQueueReceive( xStringQueue, &pcString, portMAX_DELAY );
    /* Print out the string received. */
    printf( pcString );
  }
  vTaskDelete(NULL);
}

static void vIntegerGenerator( void *pvParameters )
{
  portTickType xLastExecutionTime;
  unsigned int ulValueToSend = 0;
  int i;

  xLastExecutionTime = xTaskGetTickCount();

  for( ;; ){
    vTaskDelayUntil( &xLastExecutionTime, 200 / portTICK_RATE_MS );

    for( i = 0; i < 5; i++ ){ 
      xQueueSendToBack( xIntegerQueue, &ulValueToSend, 0 );
      ulValueToSend++;
    }
    printf( "Generator task.\r\n" );
  }
  vTaskDelete(NULL);
}


void app_main(void)
{
  xIntegerQueue = xQueueCreate( 10, sizeof( unsigned long ) );
  xStringQueue = xQueueCreate( 10, sizeof( char * ) );  

  xTaskCreate( vIntegerGenerator, "IntGen", 2000, NULL, 1, NULL );  
  xTaskCreate( vStringPrinter, "String", 2000, NULL, 2, NULL );
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


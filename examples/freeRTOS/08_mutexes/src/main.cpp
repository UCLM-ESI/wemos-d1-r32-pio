#include <Arduino.h>

#include "nvs_flash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"


QueueHandle_t xMutex;



static void prvNewPrintString( const char *pcString )
{
  xSemaphoreTake( xMutex, 2000/portTICK_PERIOD_MS  );
  {
    printf( "%s\n", pcString );
    //fflush( stdout );
  }
  xSemaphoreGive( xMutex );
}

static void prvPrintTask( void *pvParameters )
{
  char *pcStringToPrint;
  pcStringToPrint = ( char * ) pvParameters;
  
  for( ;; ){
    prvNewPrintString( pcStringToPrint );
    vTaskDelay( ( 1000/portTICK_PERIOD_MS ) );
  }
  vTaskDelete(NULL);
}


void app_main(void)
{
  xMutex = xSemaphoreCreateMutex();
  //srand( 567 );

  if( xMutex != NULL )
  {
    const char* mTask1 = "Task 1";
    const char* mTask2 = "Task 2";
    xTaskCreate( prvPrintTask, "Print1", 1000, (void *)mTask1, 1, NULL );
    xTaskCreate( prvPrintTask, "Print2", 1000, (void *)mTask2, 2, NULL );
  }
}





void setup() {
  // put your setup code here, to run once:
  app_main();
}

void loop() {
  // put your main code here, to run repeatedly:
}


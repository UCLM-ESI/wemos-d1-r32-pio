#include <Arduino.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


const char *pcTextForTask1 = "Task 1 is running\r\n";
const char *pcTextForTask2 = "Task 2 is running\t\n";
const char *pcTextForTask3 = "Task 3 is running\r\n";

void vTaskFunction( void *pvParameters )
{
  char *pcTaskName;
  pcTaskName = ( char * ) pvParameters;
  for(;;) {
    printf(pcTaskName);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


void vTask1(void* pvParam)
{
  const char *msg = "Task 1 is running\r\n";
  for(;;) {
    printf(msg);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void vTask2(void* pvParam)
{
  const char *msg = "Task 2 is running\r\n";
  for(;;) {
    printf(msg);
    //delay(1000);
    vTaskDelay(1000/portTICK_PERIOD_MS); 
  }
  vTaskDelete(NULL);
}


void vTask3(void* pvParam)
{
  const char *msg = "Task 3 is running\r\n";
 
  for(;;) {
    printf(msg); 
    vTaskDelay(1000/portTICK_PERIOD_MS); 
  }
  vTaskDelete(NULL);
}



void app_main(void)
{
  if(xTaskGetSchedulerState()==taskSCHEDULER_RUNNING)
  printf("Scheduler is running\n");
   
  xTaskCreate(vTask1, "Task 1", 1500, NULL, 3, NULL);
  xTaskCreate(vTask2, "Task 2", 1500, NULL, 3, NULL);
  xTaskCreate(vTask3, "Task 3", 1500, NULL, 3, NULL);
  
/*
  xTaskCreatePinnedToCore(vTask1, "Task 1", 1000, NULL, 10, NULL,0);
  xTaskCreatePinnedToCore(vTask2, "Task 2", 1000, NULL, 2, NULL,0);
  xTaskCreatePinnedToCore(vTask3, "Task 3", 1000, NULL, 3, NULL,0);
*/

/* 
  xTaskCreate(vTaskFunction, "Task 1", 1500, (void*)pcTextForTask1, 10, NULL);
  xTaskCreate(vTaskFunction, "Task 2", 1500, (void*)pcTextForTask2, 1, NULL);
  xTaskCreate(vTaskFunction, "Task 3", 1500, (void*)pcTextForTask3, 5, NULL);
*/

/*
  xTaskCreatePinnedToCore(vTaskFunction, "Task 1", 2000, (void*)pcTextForTask1, 10, NULL, 0);
  xTaskCreatePinnedToCore(vTaskFunction, "Task 2", 2000, (void*)pcTextForTask2, 1, NULL, 0);
  xTaskCreatePinnedToCore(vTaskFunction, "Task 3", 2000, (void*)pcTextForTask3, 5, NULL, 0);*/

  //vTaskStartScheduler();
}

void setup() {
  // put your setup code here, to run once:
  app_main();
}

void loop() {
  // put your main code here, to run repeatedly:
  //app_main();
}


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
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void vTask2(void* pvParam)
{
  const char *msg = "Task 2 is running\r\n";
  for(;;) {
    printf(msg);    
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


void vTask3(void* pvParam)
{
  const char *msg = "Task 3 is running\r\n";
 
  for(;;) {
    printf(msg);
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}



void app_main(void)
{
  TaskHandle_t t1Handler;
  if(xTaskGetSchedulerState()==taskSCHEDULER_RUNNING)
  printf("Scheduler is running\n");

  xTaskCreatePinnedToCore(vTask1, "Task 1", 1000, NULL, 1, &t1Handler,0);
  xTaskCreatePinnedToCore(vTask2, "Task 2", 1000, NULL, 2, NULL,0);
  xTaskCreatePinnedToCore(vTask3, "Task 3", 1000, NULL, 3, NULL,0);

  delay(5000);
  vTaskSuspend(t1Handler);

  delay(5000);
  vTaskResume(t1Handler);

  //vTaskStartScheduler();
}

void setup() {
  // put your setup code here, to run once:
  app_main();
}

void loop() {
  // put your main code here, to run repeatedly:
}


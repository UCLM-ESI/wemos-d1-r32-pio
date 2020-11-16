#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void vTask1(void* pvParam)
{
  for(;;) {
    int val = digitalRead(D13);
    val = (~val & 0x00000001);
    digitalWrite(D13, val);
    //vTaskDelay(1000/portTICK_PERIOD_MS);
    delay(1000);
  }
  vTaskDelete(NULL);
}

void vTask2(void* pvParam)
{
  for(;;) {
    int val = digitalRead(D12);
    val = (~val & 0x00000001);
    digitalWrite(D12, val);    
    //vTaskDelay(2000/portTICK_PERIOD_MS);
    delay(2000);
  }
  vTaskDelete(NULL);
}

void vTask3(void* pvParam)
{
  for(;;) {
    int val = digitalRead(D9);
    val = (~val & 0x00000001);
    digitalWrite(D9, val);
    digitalWrite(D10, val);
    digitalWrite(D11, val);
    //vTaskDelay(3000/portTICK_PERIOD_MS);
    delay(3000);
  }
  vTaskDelete(NULL);
}


void app_main() 
{
  pinMode(D9, OUTPUT);
  pinMode(D10, OUTPUT);
  pinMode(D11, OUTPUT);
  pinMode(D12, OUTPUT);
  pinMode(D13, OUTPUT);

  xTaskCreatePinnedToCore(vTask1, "Task 1", 1500, NULL, 5, NULL,0);
  xTaskCreatePinnedToCore(vTask2, "Task 2", 1500, NULL, 1, NULL,0);
  xTaskCreatePinnedToCore(vTask3, "Task 3", 1500, NULL, 10, NULL,0);

  // vTaskStartScheduler();
}

void setup() {
  // put your setup code here, to run once:
  app_main();
}

void loop() {
  // put your main code here, to run repeatedly:
}

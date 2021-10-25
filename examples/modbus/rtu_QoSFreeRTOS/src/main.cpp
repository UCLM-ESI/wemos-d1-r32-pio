#include <Arduino.h>
#include <ModbusRTU.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

//#define SLAVE1
//#define SLAVE2
#define MASTER

const uint8_t REG1 = 10;
const uint8_t REG2 = 20;
const uint8_t SLAVE_ID_1 = 1;
const uint8_t SLAVE_ID_2 = 2;

ModbusRTU mb;


#ifdef MASTER
static uint16_t sw1 = 0;
static uint16_t sw2 = 0;
static uint16_t coilsRS1 = 0;
static uint16_t coilsRS2 = 0;

QueueHandle_t xMutex;
xTaskHandle handlerTaskRead1;
xTaskHandle handlerTaskRead2;
xTaskHandle handlerTaskWrite1;
xTaskHandle handlerTaskWrite2;

bool cbRead_Slave1(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  if (coilsRS1 == 0)
    digitalWrite(D13,1);
  else
    digitalWrite(D13,0);
  if (event != Modbus::EX_SUCCESS)
    if (uxTaskPriorityGet(handlerTaskWrite1) == 1 &&
    uxTaskPriorityGet(handlerTaskRead2) == 1 &&
    uxTaskPriorityGet(handlerTaskWrite2) == 1)
      vTaskPrioritySet(handlerTaskRead1,10);
  else
    vTaskPrioritySet(handlerTaskRead1,1);
  return true;
}


bool cbRead_Slave2(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  if (coilsRS2 == 0)
    digitalWrite(D12,1);
  else
    digitalWrite(D12,0);
  if (event != Modbus::EX_SUCCESS)    
    if (uxTaskPriorityGet(handlerTaskWrite1) == 1 &&
    uxTaskPriorityGet(handlerTaskRead1) == 1 &&
    uxTaskPriorityGet(handlerTaskWrite2) == 1)
      vTaskPrioritySet(handlerTaskRead2,10);
  else
    vTaskPrioritySet(handlerTaskRead2,1);
  //xSemaphoreGive(xMutex);
  return true;
}

bool cbWrite_Slave1(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  if (event != Modbus::EX_SUCCESS)
    if (uxTaskPriorityGet(handlerTaskWrite2) == 1 &&
    uxTaskPriorityGet(handlerTaskRead1) == 1 &&
    uxTaskPriorityGet(handlerTaskRead2) == 1)
      vTaskPrioritySet(handlerTaskWrite1,10);
  else
    vTaskPrioritySet(handlerTaskWrite1,1);
  //xSemaphoreGive(xMutex);
  return true;
}

bool cbWrite_Slave2(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  if (event != Modbus::EX_SUCCESS)
    if (uxTaskPriorityGet(handlerTaskWrite1) == 1 &&
    uxTaskPriorityGet(handlerTaskRead1) == 1 &&
    uxTaskPriorityGet(handlerTaskRead2) == 1)
      vTaskPrioritySet(handlerTaskWrite2,10);
  else
    vTaskPrioritySet(handlerTaskWrite2,1);
  //xSemaphoreGive(xMutex);
  return true;
}
#endif


#ifdef MASTER
void IRAM_ATTR sw1_handleInterrupt(){
  sw1 = ~sw1;
}

void IRAM_ATTR sw2_handleInterrupt(){  
  sw2 = ~sw2;
}

void waitTransaction(void){
  while (mb.slave()) {
    vTaskDelay(10);
    mb.task();
  } 
}

static void vTaskRead( void *pvParameters )
{
  int slave;
  slave = (int) pvParameters;
  
  for( ;; ){
    //vTaskPrioritySet(NULL,1);
    xSemaphoreTake(xMutex, portMAX_DELAY);
    if (mb.slave()){
      xSemaphoreGive(xMutex);
      vTaskDelay(100/portTICK_PERIOD_MS);
    }
    if (slave == SLAVE_ID_1)
      mb.readHreg(SLAVE_ID_1, REG2, &coilsRS1, 1, cbRead_Slave1);
    else
      mb.readHreg(SLAVE_ID_2, REG2, &coilsRS2, 1, cbRead_Slave2);
    waitTransaction();
    xSemaphoreGive(xMutex);
    vTaskDelay(200/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


static void vTaskWrite( void* pvParameters )
{
  int slave;
  slave = (int) pvParameters;
  
  for( ;; ){
    //vTaskPrioritySet(NULL,1);
    xSemaphoreTake(xMutex, portMAX_DELAY);
    if (mb.slave()){
      xSemaphoreGive(xMutex);
      vTaskDelay(100/portTICK_PERIOD_MS);
    }
    if (slave == SLAVE_ID_1)
      if(sw1 == 0)
        mb.writeHreg(SLAVE_ID_1, REG1, (uint16_t)0, cbWrite_Slave1);
      else
        mb.writeHreg(SLAVE_ID_1, REG1, (uint16_t)1, cbWrite_Slave1);
    else
      if(sw2 == 0)
        mb.writeHreg(SLAVE_ID_2, REG1, (uint16_t)0, cbWrite_Slave2);
      else
        mb.writeHreg(SLAVE_ID_2, REG1, (uint16_t)1, cbWrite_Slave2);
    waitTransaction();
    xSemaphoreGive(xMutex);
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}


void app_main(void)
{
  xMutex = xSemaphoreCreateMutex();

  if( xMutex != NULL )
  {
    xTaskCreatePinnedToCore( vTaskRead, "Read Slave 1", 1000, (void *)SLAVE_ID_1, 1, &handlerTaskRead1, 0 );
    xTaskCreatePinnedToCore( vTaskRead, "Read Slave 2", 1000, (void *)SLAVE_ID_2, 1, &handlerTaskRead2, 0 );
    xTaskCreatePinnedToCore( vTaskWrite, "Write Slave 1", 1000, (void *)SLAVE_ID_1, 1, &handlerTaskWrite1, 0 );
    xTaskCreatePinnedToCore( vTaskWrite, "Write Slave 2", 1000, (void *)SLAVE_ID_2, 1, &handlerTaskWrite2, 0 );
  }
}

#else
static volatile int8_t sw1_slave = 0;

void IRAM_ATTR sw1_handleInterrupt(){
  if (sw1_slave == 0){
    mb.Hreg(REG2, 1);
    sw1_slave = 1;
  }
  else{
    mb.Hreg(REG2, 0);
    sw1_slave = 0;
  }
}
#endif


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mb.begin(&Serial);

  pinMode(D13, OUTPUT);
  pinMode(D2, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(D2), &sw1_handleInterrupt, FALLING);

  digitalWrite(D13,0);

#ifdef MASTER  
  pinMode(D12, OUTPUT);
  pinMode(D3, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(D3), &sw2_handleInterrupt, FALLING);

  digitalWrite(D12,0);

  mb.master();
  app_main();
#else
#ifdef SLAVE1
  mb.slave(SLAVE_ID_1);
#else
  mb.slave(SLAVE_ID_2);
#endif
  mb.addHreg(REG1);
  mb.Hreg(REG1, 0);
  mb.addHreg(REG2);
  mb.Hreg(REG2, 0);
#endif
}


void loop() {
  // put your main code here, to run repeatedly:
#ifdef MASTER
 // Do nothing  
 delay(1000);
#else
  if(mb.Hreg(REG1)==1){
    digitalWrite(D13,1);
  }
  else{
    digitalWrite(D13,0);
  }
  ///mb.Hreg(REG2, digitalRead(D2));  
  mb.task();
  yield();
  delay(200);
  #endif
}

#include <Arduino.h>
#include <ModbusRTU.h>

//#define SLAVE1
//#define SLAVE2
#define MASTER

const uint8_t REG1 = 10;
const uint8_t REG2 = 20;
const uint8_t SLAVE_ID_1 = 1;
const uint8_t SLAVE_ID_2 = 2;

ModbusRTU mb;


#ifdef MASTER
static enum {stREAD_SLAVE1, stREAD_SLAVE2, stWRITE_SLAVE1, stWRITE_SLAVE2} next_state;
static uint16_t sw1 = 0;
static uint16_t sw2 = 0;
static uint16_t coilsRS1 = 0;
static uint16_t coilsRS2 = 0;

bool cbRead_Slave1(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  if (coilsRS1 == 0)
    digitalWrite(D13,1);
  else
    digitalWrite(D13,0);
  return true;
}


bool cbRead_Slave2(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  if (coilsRS2 == 0)
    digitalWrite(D12,1);
  else
    digitalWrite(D12,0);
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

void waitTransaction(void){
  while (mb.slave()) {
    mb.task();
    delay(10);
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
#ifdef MASTER
  switch (next_state)
  {
  case stWRITE_SLAVE1:
    if (!mb.slave()){
      if(sw1 == 0)
        mb.writeHreg(SLAVE_ID_1, REG1, (uint16_t)0);
      else
        mb.writeHreg(SLAVE_ID_1, REG1, (uint16_t)1);
      waitTransaction();
    }
    next_state = stWRITE_SLAVE2;
    break;


  case stWRITE_SLAVE2:
    if (!mb.slave()){
      if(sw2 == 0)
        mb.writeHreg(SLAVE_ID_2, REG1, (uint16_t)0);
      else
        mb.writeHreg(SLAVE_ID_2, REG1, (uint16_t)1);
      waitTransaction();
    }
    next_state = stREAD_SLAVE1;
    break;


  case stREAD_SLAVE1:
    if (!mb.slave()){
      mb.readHreg(SLAVE_ID_1, REG2, &coilsRS1, 1, cbRead_Slave1);
      waitTransaction();
    }
    next_state = stREAD_SLAVE2;
    break;
    

  case stREAD_SLAVE2:
    if (!mb.slave()){
      mb.readHreg(SLAVE_ID_2, REG2, &coilsRS2, 1, cbRead_Slave2);
      waitTransaction();
    }
    next_state = stWRITE_SLAVE1;
    break;


    default:
      next_state = stWRITE_SLAVE1;
      break;
  }
  
  
  
#else
  if(mb.Hreg(REG1)==1){
    digitalWrite(D13,1);
  }
  else{
    digitalWrite(D13,0);
  }
  ///mb.Hreg(REG2, digitalRead(D2));  
  mb.task();
  delay(200);
  #endif
  yield();
}

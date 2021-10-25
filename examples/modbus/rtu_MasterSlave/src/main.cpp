#include <Arduino.h>
#include <ModbusRTU.h>

// Comment one of the following roles
//#define MASTER
#define SLAVE

const uint8_t REG1 = 10;
const uint8_t REG2 = 20;
const uint8_t SLAVE_ID = 1;

ModbusRTU mb;


#ifdef MASTER
static volatile uint16_t valueWR;
static uint16_t coilsRD;
static enum {stREAD, stWRITE} next_state;

bool cbRead(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  if (coilsRD == 0)
    digitalWrite(D13,1);
  else
    digitalWrite(D13,0);
  return true;
}

void IRAM_ATTR sw1_handleInterrupt(){
  valueWR = 1;
}

void IRAM_ATTR sw2_handleInterrupt(){  
  valueWR = 0;
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

#ifdef SLAVE
  mb.slave(SLAVE_ID);
  mb.addHreg(REG1);
  mb.Hreg(REG1, 0);
  mb.addHreg(REG2);
  mb.Hreg(REG2, 0);
#else
  pinMode(D3, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(D3), &sw2_handleInterrupt, FALLING);

  mb.master();
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
#ifdef SLAVE
  if(mb.Hreg(REG1)==1){
    digitalWrite(D13,1);
  }
  else{
    digitalWrite(D13,0);
  }
  //mb.Hreg(REG2, sw1_slave);

  mb.task();
  delay(200);
#else
  switch (next_state)
  {
  case stREAD:  
    if(!mb.slave()){
      mb.readHreg(SLAVE_ID, REG2, &coilsRD, 1, cbRead);
      next_state = stWRITE;
    }
    break;

  case stWRITE:
    if(!mb.slave()){
      if(valueWR == 0)
        mb.writeHreg(SLAVE_ID, REG1, (uint16_t)0);
      else
        mb.writeHreg(SLAVE_ID, REG1, (uint16_t)1);
    }
    next_state = stREAD;
  break;

  default:
    next_state = stREAD;
    break;
  }
  while (mb.slave()) {
    mb.task();
    delay(10);
  }
#endif
  yield();
}

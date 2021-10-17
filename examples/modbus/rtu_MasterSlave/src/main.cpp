#include <Arduino.h>
#include <ModbusRTU.h>

#define SLAVE

#define REG1 10
#define REG2 20
#define SLAVE_ID 1

ModbusRTU mb;

static uint8_t val_TX1 = 0;

#ifdef MASTER
uint16_t coils;

bool cbWrite(Modbus::ResultCode event, uint16_t transactionId, void* data) {
  return true;
}
#endif


void setup() {
  // put your setup code here, to run once:
  pinMode(D13, OUTPUT);
  pinMode(D2, INPUT_PULLDOWN);
#ifdef MASTER
  pinMode(D3, INPUT_PULLDOWN);
#endif

  Serial.begin(9600);
  mb.begin(&Serial);

#ifdef SLAVE
  mb.slave(SLAVE_ID);
  mb.addHreg(REG1);
  mb.Hreg(REG1, 0);
  mb.addHreg(REG2);
  mb.Hreg(REG2, 0);
#else
  mb.master();
#endif

  digitalWrite(D13,0);
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
  mb.Hreg(REG2, digitalRead(D2));
#else
  if (!mb.slave()){
    if(digitalRead(D2) == 0){
      coils = 1;
      mb.writeHreg(SLAVE_ID, REG1, &coils, 1, cbWrite);
    }
    if(digitalRead(D3) == 0){
      coils = 0;
      mb.writeHreg(SLAVE_ID, REG1, &coils, 1, cbWrite);
    }
    
    mb.readHreg(SLAVE_ID, REG2, &coils, 1, cbWrite);
    if (coils == 0)
      digitalWrite(D13,1);
    else
      digitalWrite(D13,0);
  }
#endif
  mb.task();
  delay(200);
}
#include <Arduino.h>


#define onSwitch D2
#define offSwitch D3



void IRAM_ATTR on_handleInterrupt(){
  noInterrupts();
  digitalWrite(D12, 1);
  interrupts()
}


void IRAM_ATTR off_handleInterrupt(){
  noInterrupts();
  digitalWrite(D12, 0);
  interrupts();
}


void setup() {
  // put your setup code here, to run once:
  pinMode(D12, OUTPUT);
  pinMode(onSwitch, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(onSwitch), &on_handleInterrupt, FALLING);
  pinMode(offSwitch, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(offSwitch), &off_handleInterrupt, FALLING); 
}



void loop() {
  // put your main code here, to run repeatedly:
}
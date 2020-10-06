#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(D12, OUTPUT);
  pinMode(D13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D12, 0);
  digitalWrite(D13, 1);
  delay(1000);
  digitalWrite(D12, 1);
  digitalWrite(D13, 0);
  delay(1000);
}
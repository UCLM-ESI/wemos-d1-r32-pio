#include <Arduino.h>
#include "analogWrite.h"


int brightness = 0;
int fadeAmount = 5;



void setup() {
  // put your setup code here, to run once:
  pinMode(D13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(D13, brightness);
  // cambiar el valor de brillo del led:
  brightness = brightness + fadeAmount;
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  delay(60);
}
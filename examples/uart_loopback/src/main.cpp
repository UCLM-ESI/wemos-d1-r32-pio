#include <Arduino.h>

static uint8_t val_TX1 = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial1.write(val_TX1);
  int val_RX2 = Serial2.read();
  Serial.printf("[UART 2]: %d\r\n", val_RX2);
  val_TX1++;
  delay(2000);
}
#include <Arduino.h>
#include <analogWrite.h>

#define redLed   D9
#define greenLed D10
#define blueLed  D11

void setup() {
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
}

void setColor(int red, int green, int blue) {
    analogWrite(redLed, red);
    analogWrite(greenLed, green);
    analogWrite(blueLed, blue);
}

void loop() {
    setColor(255, 0, 0); // red
    delay(1000);
    setColor(0, 255, 0); // green
    delay(1000);
    setColor(0, 0, 255); // blue
    delay(1000);
    setColor(255, 255, 0); // yellow
    delay(1000);
    setColor(80, 0, 80); // purple
    delay(1000);
    setColor(0, 255, 255); // aqua
    delay(1000);
    setColor(255, 255, 255); // white
    delay(1000);
    setColor(0, 0, 0); // off
    delay(1000);
}

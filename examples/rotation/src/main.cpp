#include <Arduino.h>
#include <analogWrite.h>

#define ADC_MODE2

#define rotation A0

void setup() {
    Serial.begin(9600);
    pinMode(rotation, INPUT);
    analogReadResolution(10);
}


void loop() {
   Serial.printf("Value of rotation: ");    

#ifdef ADC_MODE
    adcAttachPin(rotation);
    adcStart(rotation);
    while(adcBusy(rotation));
    Serial.println(adcEnd(rotation));
#else
    int val = analogRead(rotation);
    Serial.println(val);
#endif

    delay(1000);

}

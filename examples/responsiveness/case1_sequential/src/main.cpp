#include <Arduino.h>

#define redLed   D9
#define greenLed D10
#define blueLed  D11

#define speedSwitch D2
#define modeSwitch D3

#define W_DELAY_SLOW 18000
#define W_DELAY_FAST 8000
#define RGB_DELAY_SLOW 25000
#define RGB_DELAY_FAST 10000

static int w_delay = W_DELAY_SLOW;
static int rgb_delay = RGB_DELAY_SLOW;

void setup() {
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
    pinMode(speedSwitch, INPUT);
    pinMode(modeSwitch, INPUT);
}

void control_RGB_leds(unsigned char red, unsigned char green, unsigned char blue) {
    digitalWrite(redLed, red);
    digitalWrite(greenLed, green);
    digitalWrite(blueLed, blue);
}


void Delay(uint32_t t)
{
    volatile int n;
    while(t--){
        n=1000;
        while(n--);
    }
}

void loop() {
    
    if (digitalRead(modeSwitch) == 0){
        control_RGB_leds(1,1,1);
        Delay(w_delay);
        control_RGB_leds(0,0,0);
        Delay(w_delay);
    }
    else{
        control_RGB_leds(1,0,0);
        Delay(rgb_delay);
        control_RGB_leds(0,1,0);
        Delay(rgb_delay);
        control_RGB_leds(0,0,1);
        Delay(rgb_delay);
    }

    
    if (digitalRead(speedSwitch) == 0){
        w_delay = W_DELAY_FAST;
        rgb_delay = RGB_DELAY_FAST;
    }
    else{
        w_delay = W_DELAY_SLOW;
        rgb_delay = RGB_DELAY_SLOW;
    }
}

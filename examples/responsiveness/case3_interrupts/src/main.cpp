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

static volatile int g_w_delay = W_DELAY_SLOW;
static volatile int g_rgb_delay = RGB_DELAY_SLOW;
static volatile int g_flash = 0;



    
void IRAM_ATTR sw1_handleInterrupt(){
    if (digitalRead(speedSwitch) == 0){
        if (g_w_delay == W_DELAY_SLOW){
            g_w_delay = W_DELAY_FAST;
            g_rgb_delay = RGB_DELAY_FAST;
        }
        else{
            g_w_delay = W_DELAY_SLOW;
            g_rgb_delay = RGB_DELAY_SLOW;
        }
    }
}


void IRAM_ATTR sw2_handleInterrupt(){
    if (digitalRead(modeSwitch) == 0){
        if(g_flash == 0)
            g_flash = 1;
        else
            g_flash = 0;
    }
}


void setup() {
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
    pinMode(speedSwitch, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(speedSwitch), sw1_handleInterrupt, FALLING);
    pinMode(modeSwitch, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(modeSwitch), sw2_handleInterrupt, FALLING);
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


void Task_RGB()
{
    if (g_flash == 0){
        control_RGB_leds(1,0,0);
        Delay(g_rgb_delay);
        control_RGB_leds(0,1,0);
        Delay(g_rgb_delay);
        control_RGB_leds(0,0,1);
        Delay(g_rgb_delay);
    }
}


void Task_Flash()
{
    if(g_flash == 1){
        control_RGB_leds(1,1,1);
        Delay(g_w_delay);
        control_RGB_leds(0,0,0);
        Delay(g_w_delay);
    }
}


void loop() {
    Task_Flash();
    Task_RGB();    
}

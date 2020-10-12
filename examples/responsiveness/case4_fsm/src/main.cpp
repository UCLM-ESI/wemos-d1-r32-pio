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

static int g_w_delay = W_DELAY_SLOW;
static int g_rgb_delay = RGB_DELAY_SLOW;
static int g_flash = 0;

void Buttons_handleInterrupt(){

    if (digitalRead(modeSwitch) == 0){
        if(g_flash == 0)
            g_flash = 1;
        else
            g_flash = 0;
    }

    
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

void setup() {
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
    pinMode(speedSwitch, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(speedSwitch), Buttons_handleInterrupt, FALLING);
    pinMode(modeSwitch, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(modeSwitch), Buttons_handleInterrupt, FALLING);
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
  static enum {ST_RED,ST_GREEN,ST_BLUE,ST_NONE} next_state;

  if (g_flash == 0){
    switch (next_state)
    {
    case ST_RED:
      control_RGB_leds(1,0,0);
      Delay(g_rgb_delay);
      next_state = ST_GREEN;
      break;
    
    case ST_GREEN:
      control_RGB_leds(0,1,0);
      Delay(g_rgb_delay);
      next_state = ST_BLUE;
      break;

    case ST_BLUE:
      control_RGB_leds(0,0,1);
      Delay(g_rgb_delay);
      next_state = ST_RED;
      break;

    default:
      next_state = ST_RED;
      break;
    }
  }
}

void Task_Flash()
{
  static enum {ST_WHITE,ST_BLACK,ST_NONE} next_state;

  if (g_flash == 1){
    switch (next_state)
    {
    case ST_WHITE:
      control_RGB_leds(1,1,1);
      Delay(g_w_delay);
      next_state = ST_BLACK;
      break;

    case ST_BLACK:
      control_RGB_leds(0,0,0);
      Delay(g_w_delay);
      next_state = ST_WHITE;
      break;

  default:
      next_state = ST_WHITE;
      break;
    }
  }
}


void loop() {
    Task_Flash();
    Task_RGB();    
}

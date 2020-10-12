#include <Arduino.h>

#define redLed   D9
#define greenLed D10
#define blueLed  D11

#define speedSwitch D2
#define modeSwitch D3

#define W_DELAY_SLOW 1800000
#define W_DELAY_FAST 800000
#define RGB_DELAY_SLOW 2500000
#define RGB_DELAY_FAST 1000000

static int g_w_delay = W_DELAY_SLOW;
static int g_rgb_delay = RGB_DELAY_SLOW;
static int g_flash = 0;
volatile uint8_t g_enTime = 0;
volatile uint32_t g_timeCnt = 0;
volatile uint32_t g_time = 0;
hw_timer_t * timer = NULL;

static enum {ST_RED,ST_RED_WAIT,ST_GREEN,ST_GREEN_WAIT,ST_BLUE,ST_BLUE_WAIT,ST_NONE} next_stateRGB;
static enum {ST_WHITE,ST_WHITE_WAIT,ST_BLACK,ST_BLACK_WAIT} next_stateF;

void IRAM_ATTR Timer_handleInterrupt(){
  if(g_enTime == 1)
    g_timeCnt++;
}

void IRAM_ATTR Buttons_handleInterrupt(){

    if (digitalRead(modeSwitch) == 0){
        if(g_flash == 0)
            g_flash = 1;
        else{
            g_flash = 0;
            next_stateF = ST_WHITE;
        }
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
        g_timeCnt = g_time;
    }
}

void setup() {
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
    pinMode(speedSwitch, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(speedSwitch), &Buttons_handleInterrupt, FALLING);
    pinMode(modeSwitch, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(modeSwitch), &Buttons_handleInterrupt, FALLING);
    timer = timerBegin(0, 3999, true);
    timerAttachInterrupt(timer, &Timer_handleInterrupt, true);
    timerAlarmWrite(timer, 10, true);
    timerAlarmEnable(timer);
}

void control_RGB_leds(unsigned char red, unsigned char green, unsigned char blue) {
    digitalWrite(redLed, red);
    digitalWrite(greenLed, green);
    digitalWrite(blueLed, blue);
}


void initTime(uint32_t t)
{
  g_timeCnt = 0;
  g_enTime = 1;
  g_time = t/1000;
}

void stopTime()
{
  g_enTime = 0;
}

uint8_t expiredTime()
{
  if(g_time <= g_timeCnt)
    return 1;
  else
    return 0;
}


void Task_RGB()
{
  if (g_flash == 0){
    switch (next_stateRGB)
    {
    case ST_RED:
      control_RGB_leds(1,0,0);
      initTime(g_rgb_delay);
      next_stateRGB = ST_RED_WAIT;
      break;

    case ST_RED_WAIT:
      if(expiredTime() == 1){
        stopTime();
        next_stateRGB = ST_GREEN;
      }
      break;
    
    case ST_GREEN:
      control_RGB_leds(0,1,0);
      initTime(g_rgb_delay);
      next_stateRGB = ST_GREEN_WAIT;
      break;

    case ST_GREEN_WAIT:
      if(expiredTime() == 1){
        stopTime();
        next_stateRGB = ST_BLUE;
      }
      break;

    case ST_BLUE:
      control_RGB_leds(0,0,1);
      initTime(g_rgb_delay);
      next_stateRGB = ST_BLUE_WAIT;
      break;

    case ST_BLUE_WAIT:
      if(expiredTime() == 1){
        stopTime();
        next_stateRGB = ST_RED;
      }
      break;

    default:
      next_stateRGB = ST_RED;
      break;
    }
  }
}

void Task_Flash()
{
  if (g_flash == 1){
    switch (next_stateF)
    {
    case ST_WHITE:
      control_RGB_leds(1,1,1);
      initTime(g_w_delay);
      next_stateF = ST_WHITE_WAIT;
      break;

    case ST_WHITE_WAIT:
      if(expiredTime() == 1){
        stopTime();
        next_stateF = ST_BLACK;
      }      
      break;

    case ST_BLACK:
      control_RGB_leds(0,0,0);
      initTime(g_w_delay);
      next_stateF = ST_BLACK_WAIT;
      break;

    case ST_BLACK_WAIT:
      if(expiredTime() == 1){
        stopTime();
        next_stateF = ST_WHITE;
      }      
      break;

  default:
      next_stateF = ST_WHITE;
      break;
    }
  }
}


void loop() {
    Task_Flash();
    Task_RGB();    
}

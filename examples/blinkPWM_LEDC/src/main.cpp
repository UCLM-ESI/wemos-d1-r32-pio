#include <Arduino.h>
#include "driver/ledc.h"


//#define ESPIDF


int brightness = 0;
int fadeAmount = 5;


void setup() {
  // put your setup code here, to run once:
#ifdef ESPIDF
  ledc_timer_config_t ledc_timer = { 
    LEDC_LOW_SPEED_MODE, // timer mode
    LEDC_TIMER_8_BIT, // resolution of PWM duty
    LEDC_TIMER_0,            // timer index
    80000};                      // frequency of PWM signal

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel ={
      GPIO_NUM_18,
      LEDC_LOW_SPEED_MODE,
      LEDC_CHANNEL_0,
      LEDC_INTR_DISABLE,
      LEDC_TIMER_0,
      0,
      0
    };
    ledc_channel_config(&ledc_channel);

    // Initialize fade service.
    ledc_fade_func_install(0);
#else
  // configure LED PWM functionalitites
  ledcSetup(0, 5000, 8);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(D13, 0);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
#ifdef ESPIDF 
  //ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, brightness);
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 255);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

  // cambiar el valor de brillo del led:
  brightness = brightness + fadeAmount;
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  delay(60);
#else
  ledcWrite(0, brightness);
  // cambiar el valor de brillo del led:
  brightness = brightness + fadeAmount;
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount ;
  }
  delay(60);
#endif
}
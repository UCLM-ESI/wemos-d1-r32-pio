#include <Arduino.h>
#include "driver/ledc.h"


//#define ESPIDF


int brightness = 0;
int fadeAmount = 5;


void setup() {
  // put your setup code here, to run once:
#ifdef ESPIDF
  
  ledc_timer_config_t ledc_timer; 
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE; // timer mode
    ledc_timer.duty_resolution = LEDC_TIMER_8_BIT; // resolution of PWM duty
    ledc_timer.timer_num = LEDC_TIMER_0;            // timer index
    ledc_timer.freq_hz = 80000;                      // frequency of PWM signal

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel;
      ledc_channel.gpio_num = GPIO_NUM_18;
      ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
      ledc_channel.channel = LEDC_CHANNEL_0;
      ledc_channel.intr_type = LEDC_INTR_DISABLE;
      ledc_channel.timer_sel = LEDC_TIMER_0;
      ledc_channel.duty = 0;
      ledc_channel.hpoint = 0;
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
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, brightness);
  //ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 255);
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

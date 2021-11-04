#include <Arduino.h>
#include <driver/dac.h>
#include <driver/adc.h>


//#define ESPIDF

static char volt = 0;
static char delta = 10;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

#ifdef ESPIDF
  dac_output_enable(DAC_CHANNEL_2);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten((adc1_channel_t)ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
#else
  pinMode(GPIO_NUM_34, INPUT);
  pinMode(DAC_CHANNEL_2_GPIO_NUM, OUTPUT);
#endif
}

void loop() {
  // put your main code here, to run repeatedly:

#ifdef ESPIDF
  dac_output_voltage(DAC_CHANNEL_2,volt);
  int val = adc1_get_raw(ADC1_CHANNEL_6);
#else
  dacWrite(DAC_CHANNEL_2_GPIO_NUM,volt);
  int val = analogRead(GPIO_NUM_34);
#endif

  Serial.printf("Read val: %d\r\n",val);

  delay(1000);
  volt += delta;
}

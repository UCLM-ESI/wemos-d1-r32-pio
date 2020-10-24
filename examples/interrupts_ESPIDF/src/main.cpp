#include <Arduino.h>


#define onSwitch D2
#define offSwitch D3

static volatile bool g_flash;


static void IRAM_ATTR off_isr_handler(void* arg){
  uint32_t gpio_num = (uint32_t) arg;
  if(gpio_num == 25){
    gpio_set_level(GPIO_NUM_18, 1);
    gpio_set_level(GPIO_NUM_19, 1);
  }
  else{
    if(gpio_num == 26){
      gpio_set_level(GPIO_NUM_18, 0);
      gpio_set_level(GPIO_NUM_19, 0);
    }   
  }
}


void setup() {
  // put your setup code here, to run once:
  /*gpio_config_t io_conf;
  //disable interrupt
  io_conf.intr_type = GPIO_INTR_DISABLE;
  //set as output mode
  io_conf.mode = GPIO_MODE_OUTPUT;
  //bit mask of the pins that you want to set,e.g.GPIO18
  io_conf.pin_bit_mask = ((1ULL<<GPIO_NUM_18)|(1ULL<<GPIO_NUM_19)); 
  //disable pull-down mode
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  //disable pull-up mode
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  //configure GPIO with the given settings
  gpio_config(&io_conf);
*/
  
  gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT);
  gpio_pullup_dis(GPIO_NUM_18);
  gpio_pulldown_dis(GPIO_NUM_18);

  gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT);
  gpio_pullup_dis(GPIO_NUM_19);
  gpio_pulldown_dis(GPIO_NUM_19);


  //interrupt of rising edge
  //gpio_config_t io_conf;
/*  io_conf.intr_type = GPIO_INTR_POSEDGE;
  //bit mask of the pins, use GPIO4/5 here
  io_conf.pin_bit_mask = ((1ULL<<GPIO_NUM_25) | (1ULL<<GPIO_NUM_26));
  //set as input mode    
  io_conf.mode = GPIO_MODE_INPUT;
  //enable pull-up mode
  io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
  gpio_config(&io_conf);
*/

  gpio_set_direction(GPIO_NUM_26, GPIO_MODE_INPUT);
  gpio_pullup_en(GPIO_NUM_26);

  gpio_set_direction(GPIO_NUM_25, GPIO_MODE_INPUT);
  gpio_pullup_en(GPIO_NUM_25);


  gpio_set_intr_type(GPIO_NUM_25, GPIO_INTR_POSEDGE);
  gpio_set_intr_type(GPIO_NUM_26, GPIO_INTR_POSEDGE);
  gpio_install_isr_service(0);
  gpio_isr_handler_add(GPIO_NUM_25, off_isr_handler, (void*)GPIO_NUM_25);
  gpio_isr_handler_add(GPIO_NUM_26, off_isr_handler, (void*)GPIO_NUM_26);
  
  g_flash = false;
}



void loop() {
  // put your main code here, to run repeatedly:
}
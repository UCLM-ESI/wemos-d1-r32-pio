#ifndef Pins_Arduino_h
#define Pins_Arduino_h

//              Arduino  WEMOS
// SW1             D2      26
// SW2             D3      25
// buzzer          D5      16
// IR              D6      27
// RGB red         D9      13
// RGB green       D11      5
// RGB blue        D10     23
// LED4 (red)      D12     19
// LED3 (blue)     D13     18

// pontenciometer  A0       2
// LDR             A1       4
// temperarture    A2      36

#include <stdint.h>

#define EXTERNAL_NUM_INTERRUPTS 16
#define NUM_DIGITAL_PINS        40
#define NUM_ANALOG_INPUTS       16

#define analogInputToDigitalPin(p)  (((p)<20)?(esp32_adc2gpio[(p)]):-1)
#define digitalPinToInterrupt(p)    (((p)<40)?(p):-1)
#define digitalPinHasPWM(p)         (p < 34)

static const uint8_t LED_BUILTIN = 2;
#define BUILTIN_LED  LED_BUILTIN // backward compatibility

static const uint8_t TX = 1;
static const uint8_t RX = 3;

static const uint8_t SDA = 21;
static const uint8_t SCL = 22;

static const uint8_t SS    = 5;
static const uint8_t MOSI  = 23;
static const uint8_t MISO  = 19;
static const uint8_t SCK   = 18;

static const uint8_t D2  = 26;
static const uint8_t D3  = 25;
static const uint8_t D4  = 17;
static const uint8_t D5  = 16;
static const uint8_t D6  = 27;
static const uint8_t D7  = 14;
static const uint8_t D8  = 12;
static const uint8_t D9  = 13;
static const uint8_t D10 = 5;
static const uint8_t D11 = 23;
static const uint8_t D12 = 19;
static const uint8_t D13 = 18;
static const uint8_t D14 = 21;
static const uint8_t D15 = 22;

static const uint8_t A0 = 2;
static const uint8_t A1 = 4;
static const uint8_t A2 = 35;
static const uint8_t A3 = 34;
static const uint8_t A4 = 36;
static const uint8_t A5 = 39;


static const uint8_t IO0  = 0;
static const uint8_t IO15 = 15;
static const uint8_t IO32 = 32;
static const uint8_t IO33 = 33;


static const uint8_t CLK = 6;
static const uint8_t SD0 = 7;
static const uint8_t SD1 = 8;
static const uint8_t SD2 = 9;
static const uint8_t SD3 = 10;
static const uint8_t CMD = 11;

#endif /* Pins_Arduino_h */

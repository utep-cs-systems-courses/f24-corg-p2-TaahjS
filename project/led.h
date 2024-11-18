#ifndef led_included
#define led_included

#endif

#ifndef buzzer_included
#define buzzer_included

#include <msp430.h>
#include "timerLib/libTimer.h"

#define LED_GREEN BIT0
#define LED_RED BIT6
#define LEDS (BIT0 | BIT6)

#define SW1 BIT3
#define SWITCHES SW1

void led_init();
void led_update();

void buzzer_init();
void buzzer_set_period(short cycles);

void switch_init();
int switch_interrupt_handler();

extern unsigned char leds_changed, green_led_state, red_led_state; //boolean flags

#endif

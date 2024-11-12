#ifndef led_included
#define led_included

#ifndef buzzer_included
#define buzzer_included

#include <msp430.h>
#include "libtimer.h"

#define LED_RED BIT0
#define LED_GREEN BIT6
#define LEDS (BIT0 | BIT6)

#define SW1 BIT3
#define SWITCHES SW1

void led_int();
void led_update();

void buzzer_init();
void buzzer_set_period(short cycles);

void switch_init();
void switch_interrupt_handler();

extern unsigned char leds_changed, green_led_state, red_led_state; //boolean flags

#endif
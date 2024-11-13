//This program turns off both red and green.
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  //Only the back LEDs matter, not the one next to the orange LED.
  //They are switched. The green LED is matched to LED_RED, and the red led is matched to LED_GREEN
  P1DIR |= LEDS; //"LEDS" refers to the bits set in the led.h file. This is prep for P1OUT
  P1OUT &= ~(LED_GREEN); //turns off green
  P1OUT &= ~(LED_RED);
  
  //P1OUT |= LED_RED; //turns on red
  //P1OUT |= LED_GREEN;

  //P1OUT &= ~(LED_GREEN | LED_RED); //running this after turning on both makes light dim, not off

  or_sr(0x18);		/* CPU off, GIE on */
}

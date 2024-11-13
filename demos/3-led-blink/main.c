//This program will turn off green, and then turn off red
//Then, the two LEDs will turn on/off every second.
//Since green started off and red started on, they will never be on/off at the same time.
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);			/* CPU off, GIE on */
}

// global state var to count time
int secondCount = 0;

//this runs 250 times a second.
void
__interrupt_vec(WDT_VECTOR) WDT()
{
  secondCount ++;
  //the lower the number, the faster the blink. 125 blinks twice every second.
  if (secondCount >= 250) { //if secondCount has run 250 times, a second has passed.
    secondCount = 0;
    P1OUT ^= LED_GREEN; //for some reason, this creates the blinking effect
    P1OUT ^= LED_RED;
  }
} 


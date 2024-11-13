//When the program begins, green will be turned off, and red will be turned on.
//The interrupt function (running 250 times per second) will continuously turn green on/off
//It will be happening so fast that it looks like green is flickering
//The function will keep green off during most interrupts, so it'll appear dimmer than red
//Every second, the amount of 'offs' during the 250 interrupts will increase
//After 10 seconds, the amount of 'offs' will be reset to its original value.
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

// global state vars that control blinking
int blinkLimit = 5;  // duty cycle = 1/blinkLimit
int blinkCount = 0;  // cycles 0...blinkLimit-1
int secondCount = 0; // state var representing repeating time 0…1s

void
__interrupt_vec(WDT_VECTOR) WDT()
{
  blinkCount ++;
  if (blinkCount >= blinkLimit) { //this will run (250/blinkLimit) times per second
    blinkCount = 0;
    P1OUT |= LED_GREEN;
  } else //this will run 250 - (250/blinkLimit) times per second	       
    P1OUT &= ~LED_GREEN;

  //function runs 250 times per second
  secondCount ++;
  if (secondCount >= 250) {
    secondCount = 0;
    blinkLimit ++;	     // every second, increase the blink limit
    if (blinkLimit >= 11)     // if the limit is 11, reset it to 5.
      blinkLimit = 5;
  }
} 


//This program does the same thing as the 4th demo, but implements functions
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

void greenControl(int on)
{
  if (on) { //if on == 1
    P1OUT |= LED_GREEN;
  } else { //if on == 0
    P1OUT &= ~LED_GREEN;
  }
}

// blink state machine
static int blinkLimit = 5;   //  state var representing reciprocal of duty cycle 
void blinkUpdate() // called every 1/250s to blink with duty cycle 1/blinkLimit
{
  static int blinkCount = 0; // state var representing blink state
  blinkCount ++;
  if (blinkCount >= blinkLimit) {
    blinkCount = 0;
    greenControl(1); //P1OUT |= LED_GREEN
  } else
    greenControl(0); //P1OUT &= ~LED_GREEN
}

void oncePerSecond() //When 1 second has passed, increase the limit unless it is 11
{
  blinkLimit ++;
  if (blinkLimit >= 11)
    blinkLimit = 5;
}

void secondUpdate()  //this is called 250 times per second. When 250 interrupts have passed.
{
  static int secondCount = 0; //keeps track of the 250 interrupts
  secondCount ++;
  if (secondCount >= 250) { //250 = 1 second
    secondCount = 0;
    oncePerSecond(); //increase blink limit unless it is 11, then set it to 5.
  } }

void timeAdvStateMachines() //called 250 times per second (so unnecessary)
{
  blinkUpdate();
  secondUpdate();
}


void __interrupt_vec(WDT_VECTOR) WDT()	//250 times per second
{
  timeAdvStateMachines();
} 
 

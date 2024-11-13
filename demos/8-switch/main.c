#include <msp430.h>
#include "libTimer.h"

#define LED_GREEN BIT0               // P1.0
#define LED_RED BIT6             // P1.6
#define LEDS (BIT0 | BIT6)

#define SW1 BIT3		//This refers to P1.3 on the MSP430, not the sound board
#define SWITCHES SW1		//MSP430 only has one switch

void main(void) 
{  
  configureClocks();

  P1DIR |= LEDS;
  P1OUT &= ~LEDS; //applies to both red & green
  
  P1REN |= SWITCHES;		//enables resistors for SWITCHES
  P1IE |= SWITCHES;		//enables interrupts for SWITCHES
  P1OUT |= SWITCHES;		//enables pull-up for SWITCHES
  //this means red won't be on until we button press at least once
  P1DIR &= ~SWITCHES;		//turns off SWITCHES bits so we can do input

  or_sr(0x18);  // CPU off, GIE on
} 

void
switch_interrupt_handler()
{
  char p1val = P1IN;		//SWITCH in P1 (MSP430)

  //allows switch interrupt sense to detect changes in current button(s)
  P1IES |= (p1val & SWITCHES);	//switch up, sense down
  P1IES &= (p1val | ~SWITCHES);	//switch down, sense up

  if (p1val & SW1) { //if switch is up (after we've pressed once)
    P1OUT |= LED_RED; //red is on
    P1OUT &= ~LED_GREEN; //green is off
  } else { //if switch is down
    P1OUT |= LED_GREEN; //green is on
    P1OUT &= ~LED_RED; //red is off
  }
}

void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHES) { //checks if interrupt was caused by button press
    P1IFG &= ~SWITCHES; //clear button interrupt
    switch_interrupt_handler();	//handles all switches
  }
}

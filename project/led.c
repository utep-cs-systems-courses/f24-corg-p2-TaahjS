#include <msp430.h>
#include "led.h"
#include "timerLib/libTimer.h"

void led_init(){ //sets up P1OUT and turns off both LEDs
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
}

void buzzer_init(){
  timerAUpmode();
  P2SEL2 &= ~(BIT6 | BIT7);
  P2SEL &= ~BIT7;
  P2SEL |= BIT6;
  P2DIR = BIT6;
}

void buzzer_set_period(short cycles){ //how frequently the buzzer buzzes
  CCR0 = cycles;
  CCR1 = cycles >> 1;
}

void switch_init(){
  P1REN |= SWITCHES;
  P1IE |= SWITCHES;
  P1OUT |= SWITCHES;
  P1DIR &= ~SWITCHES;
}

int switch_interrupt_handler(){
  char p1val = P1IN;
  P1IES |= (p1val & SWITCHES);
  P1IES &= (p1val | ~SWITCHES);

  if(p1val & SW1){ //not pressed
    return 0;
  }else{ //pressed
    return 1;
  }
}


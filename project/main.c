#include <msp430.h>
#include "timerLib/libTimer.h"
#include "led.h"

int show_pattern();

void main(void){
  configureClocks();
  switch_init();
  led_init();
  buzzer_init();
  buzzer_set_period(0);
  enableWDTInterrupts();

  

  or_sr(0x18);
}

/*
  State #1 = game is showing the light pattern. input is not accepted.
  State #2 = game is waiting for user input to follow pattern.
             the moment state 2 begins, save current second in var.
	     if seconds ever becomes 3+ greater than curr, return.
  State #3 = game is waiting for user input to end/restart game.
*/
int interrupts = 0; //250 = 1 second
int seconds = 0;

int patternClock = 0; //this value is for whenever we need to store current time
int pattern[6]; //first 5 spots are for pattern, 6th is for iteration

int state = 0; //current state. 0 = program just started, 1 = show pattern, 2 input

void
__interrupt_vec(WDT_VECTOR) WDT(){
  interrupts++;
  if(interrupts >= 250){
    seconds++;
    interrupts = 0;
  }
 end:
  if(state == 0){ //if we've just started
    patternClock = seconds; //mark current time
    pattern[0] = show_pattern(); //show first part of pattern
    pattern[5] = 1; //prepare for 2nd part of pattern
    state = 1; //transition state to show pattern
  }else if(state == 1){ //pattern display state
    if(pattern[5] == 5){ //if we've covered the entire pattern
      patternClock = seconds; //mark time
      pattern[5] = 0; //this is for input
      state = 2; //input state
    }
    if(seconds > patternClock){ //if 1 second has passed
      patternClock = seconds;
      pattern[pattern[5]] = show_pattern(); //puts next light into pattern array
      pattern[5]++; //move onto next LED in pattern
    }
  }else if(state == 2){ //input state
    if(pattern[5] == 5){ //done with input
      state = 3;
      goto end;
    }
    if(switch_update_interrupt_handler()){ //green
      P1OUT |= LED_GREEN;
      P1OUT &= ~LED_RED;
      if(pattern[pattern[5]] == 1){ //green was correct
	//make noise
	pattern[5]++;
      }else{
	//make noise
	state = 3;
      }
    }else{ //red
      P1OUT |= LED_RED;
      P1OUT &= ~LED_GREEN;
      if(seconds > patternClock && pattern[pattern[5]] == 0){ //if 1 second passed & pattern red
	//make noise
	patternClock = seconds;
	pattern[5]++;
      }else{ //pattern wasn't red and still here
	//make noise
	state = 3;
      }
    }
  }else if(state == 3){
    P1OUT &= ~LEDS;
  }
}

int show_pattern(){
  //this is supposed to randomly turn on one led and turn off the other
  if(seconds % 3 == 0){
    P1OUT |= LED_GREEN;
    P1OUT &= ~LED_RED;
    return 1;
  }
  P1OUT |= LED_RED;
  P1OUT &= ~LED_GREEN;
  return 0;
}

#include <msp430.h>
#include "timerLib/libTimer.h"
#include "led.h"

int show_pattern();
void red_on();
void green_on();
void celebrate();
void boo();

void main(void){
  configureClocks();
  switch_init(); //enable switch
  led_init(); //enable LEDs for P1OUT & turn them off
  buzzer_init(); //enable buzzer
  buzzer_set_period(0); //turn off buzzer
  enableWDTInterrupts(); //begin program w/ interrupts

  or_sr(0x18);
}

//global vars
int interrupts = 0; //250 = 1 second
int seconds = 0;

int patternClock = 0; //used to store currentTime to tell how much time has passed since an event
int pattern[6]; //stores light pattern in 1st 5 slots, the 6th tells us which pattern we're on

int state = 1; //1 = showing pattern, 2 = user repeats pattern, 3 = end

//runs 250 times per second
void
__interrupt_vec(WDT_VECTOR) WDT(){
  interrupts++;
  if(interrupts >= 250){
    seconds++;
    interrupts = 0;
  }
  
  if(state == 1){ //pattern display state
    if(pattern[5] == 5){ //5 lights have shown
      patternClock = seconds;
      pattern[5] = 0;
      state = 2;
    }else if(seconds > patternClock || pattern[5] == 0){ //1 second passed OR program just started
      patternClock = seconds;
      pattern[pattern[5]] = show_pattern(); //displays a light, puts light val into empty slot
      pattern[5]++;
    }
    
  }else if(state == 2){ //input state. from now on, pattern[5] will match the input we're on
    if(pattern[5] == 5){ //done with input
      state = 3; 
      
    }else if(switch_update_interrupt_handler()){ //switch pressed, show green , turn off red.
      green_on();
      if(pattern[pattern[5]] == 1){ //green was correct
	celebrate();
	patternClock = seconds;
	pattern[5]++; //move on
      }else{ //green was incorrect, turn off program.
	boo();
	state = 3;
      }
      
   }else{ //switch not pressed, show red, turn off green.
      red_on();
      if(seconds > patternClock && pattern[pattern[5]] == 0){ //if 1 second passed & pattern red
	celebrate();
	patternClock = seconds;
	pattern[5]++;
      }else{ //1 second passed, and red wasn't correct answer so turn off program
	boo();
	state = 3;
      }
    }
    
  }else if(state == 3){
    P1OUT &= ~LEDS;
  }
}

int show_pattern(){
  //this is supposed to randomly turn on one led and turn off the other
  if(seconds % 2 == 0){
    green_on();
    return 1;
  }
  red_on();
  return 0;
}

void red_on(){
  P1OUT |= LED_RED;
  P1OUT &= ~LED_GREEN;
}

void green_on(){
  P1OUT |= LED_GREEN;
  P1OUT &= ~LED_RED;
}

void celebrate(){
  //flash bright lights repeatedly
  //high pitch
}

void boo(){
  //flash dim lights
  //low pitch
}

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
    if(pattern[5] == 5 && seconds > patternClock){ //5 lights have shown & 1 second has passed
      P1OUT &= ~LEDS;
      patternClock = seconds;
      pattern[5] = -1;
      state = 2;
    }else if(seconds > patternClock || pattern[5] == 0){ //1 second passed OR program just started
      patternClock = seconds; //mark time
      pattern[pattern[5]] = show_pattern(); //displays 1 light, puts light val into array
      pattern[5]++; //tells where next light should be put in array
    }
    
  }else if(state == 2){ //input state. from now on, pattern[5] will match the input we're on
    if(pattern[5] == 5){ //done with input
      state = 3;
      
    }else if(pattern[5] == -1){ //if we've just entered this state, give user some time to prepare
      buzzer_set_period(500);
      if(seconds > patternClock + 1){
	buzzer_set_period(0);
	patternClock = seconds;
	pattern[5]++;
      }
  
    }else if(switch_update_interrupt_handler()){ //switch pressed, show green, turn off red.
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
      }else if (seconds > patternClock){ //if red wasn't the right answer after 1s of not press
	boo();
	state = 3;
      }
    }

  }else if(state == 3){ //game is over
   buzzer_set_period(0);
   P1OUT &= ~LEDS;
   return;
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
  P1OUT |= LEDS;
  int i = 0;
  while(patternClock == seconds){
    if(i < 5){
      buzzer_set_period(2000);
      i++;
    }else{
      buzzer_set_period(3000);
      i++;
      if(i == 10){
	i = 0;
      }
    }
  }
  P1OUT &= ~LEDS;
  buzzer_set_period(0);
}

void boo(){
  while(seconds != (patternClock + 3){ //dim lights
      buzzer_set_period(300);
      red_on();
      green_on();
  }
  buzzer_set_period(0);
  P1OUT &= ~LEDS;
}

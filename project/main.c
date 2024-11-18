#include <msp430.h>
#include "timerLib/libTimer.h"
#include "led.h"

int show_light(); //show singular light for pattern
int show_pattern();
int accept_input();
void end_program();

int red_on();
int green_on();

void celebrate();
void boo();

int interrupts = 0, seconds = 0, timeMarker = 0, state = 1;
int pattern[] = {0, 0, 0, 0, 0, 0};
int input[] = {0, 0, 0, 0, 0};

void main(void){
  configureClocks();
  switch_init(); //enable switch
  led_init(); //enable LEDs for P1OUT & turn them off
  buzzer_init(); //enable buzzer
  buzzer_set_period(0); //turn off buzzer
  enableWDTInterrupts(); //begin program w/ interrupts

  or_sr(0x18);
}
void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCHES) { //checks if switch triggered interrupt
    P1IFG &= ~SWITCHES; //clear pending switch interrupts
    if(state == 2){
      state = accept_input();
    }
  }
}

//runs 250 times per second
void
__interrupt_vec(WDT_VECTOR) WDT(){
   interrupts++;
   if(interrupts >= 250){
     seconds++;
     interrupts = 0;
   }

   if(state == 1){
     state = show_pattern();
   }
   if(state == 3){
     end_program();
   }
   
}

int show_light(){
  if(seconds % 2 == 0){
    return green_on();
  }
  return red_on();
}

int show_pattern(){
  if(pattern[5] == 0 || seconds > timeMarker){ //if beginning of program or 1s has passed
    timeMarker = seconds;
    if(pattern[5] == 5){ //if all 5 lights have shown
      pattern[5] = 0;
      P1OUT &= ~LEDS;
      return ++state; //progress state
    }
    pattern[pattern[5]] = show_light(); //save current light
    pattern[5]++;
  }
  return state; 
}

int accept_input(){
  if(pattern[5] == 5){ //done with input
    return ++state;
  }else if(switch_interrupt_handler()){ //pressed = green
    pattern[pattern[5]] = green_on();
  }else{ //not pressed = red
    pattern[pattern[5]] = red_on();
  }
  pattern[5]++;
  return state;
}

void end_program(){
  P1OUT &= ~LEDS;
  for(int i = 0; i < 6; i++){
    if(input[i] != pattern[i]){
      boo();
      return;
    }
  }
  celebrate();
}

int red_on(){
  P1OUT |= LED_RED;
  P1OUT &= ~LED_GREEN;
  return 0;
}

int green_on(){
  P1OUT |= LED_GREEN;
  P1OUT &= ~LED_RED;
  return 1;
}

void celebrate(){
  //buzzer_set_period(1000);
  P1OUT |= LEDS;
}

void boo(){
  //buzzer_set_period(500)
  P1OUT |= LEDS;
  P1OUT &= ~LEDS;
}

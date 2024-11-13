#include <msp430.h>
#include "timerLib/libTimer.h"
#include "led.h"

void main(void){
  configureClocks();
  switch_init();
  led_init();
  buzzer_init();
  buzzer_set_period(0);

  or_sr(0x18);
}

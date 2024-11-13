#ifndef led_included
#define led_included
//Somehow, the two LEDs were mixed up. Green was red and red was green until I fixed it.
#define LED_GREEN BIT0               // P1.0
#define LED_RED BIT6             // P1.6
#define LEDS (BIT0 | BIT6)

#endif // included

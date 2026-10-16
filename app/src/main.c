/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 500

int main(void) {
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }
  /*
    Create a 4-bit binary counter using all 4 LEDs.
    An LED being off should represent a binary 0, and an LED being on should represent a binary 1.
    The internal counter should increment each time BTN0 is pressed. 
    The LEDs should reset once the internal counter reaches 16.
  */
  uint8_t adder = 0;

  while (1) {
    // check buttons
    if (BTN_is_pressed(0)){
        adder = adder + 1;
    }else if (BTN_is_pressed(1)){
        adder = adder - 1;
    }else{
      ; // No operation.
    }

    // constraint
    if (adder >= 16) {
      adder = 0;
    }
   
    // turn off leds
    LED_set(0,0);
    LED_set(1,0);
    LED_set(2,0);
    LED_set(3,0);
    
    // turn on sum leds
    for (uint8_t bit = 0; bit < 4; bit++) {
      LED_set(bit, (adder >> bit) & 1);
    }

    k_msleep(SLEEP_MS);
  }
  return 0;
}

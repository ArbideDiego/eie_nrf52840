/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 10

int main(void) {
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }
  /*
    Challenge: 1. Create a password system that uses BTN0, BTN1, BTN2 as inputs, and BTN3 as the "enter" button.
    The program should start in a locked state with LED0 turned on.
    The user should be able to click a combination of BTN0, BTN1, BTN2 (that you choose at compile time) as the passkey 
    entry stage. Once the user clicks BTN3, "Correct!" should be printed if the password was correct, and "Incorrect!" 
    should be printed if the password was wrong. 

    After the user entered their password with BTN3, LED0 should turn off and your program should enter a "waiting" 
    state. The user should be able to press any button to reset the program back to its locked state 
    while it’s in the waiting state, and the program should reset.

    When the board starts, turn LED3 on for 3 seconds. During those 3 seconds,
    the user should be able to click BTN3, which puts the program into an "entry" mode until BTN3 is clicked again. 
    The user should be able to enter a password of their choice by entering a sequence of BTN0, BTN1, BTN2.
    Once they click BTN3, the password should be saved, and the program should enter the locked state,
    where it should function the same way as part 1 of this exercise.
  */
  // turn off leds
  LED_set(0, 0);
  LED_set(1, 0);
  LED_set(2, 0);
  LED_set(3, 0);

  // set led 0
  LED_set(0, 1);

  char password[] = "210";
  char entered[16] = {0};
  uint8_t entered_len = 0;
  bool waiting = false;

  while (1) {
    if (waiting) {

      if (BTN_check_clear_pressed(0) || BTN_check_clear_pressed(1) || BTN_check_clear_pressed(2) || BTN_check_clear_pressed(3)) {
        entered_len = 0;
        entered[0] = '\0';
        waiting = false;
        LED_set(0, 1);
        LED_set(1, 0);
        LED_set(2, 0);
        LED_set(3, 0);
      }
      k_msleep(SLEEP_MS);
      continue;
    }

    if (BTN_check_clear_pressed(0)) {
      if (entered_len < (sizeof(entered) - 1)) {
        entered[entered_len++] = '0';
      }
    } else if (BTN_check_clear_pressed(1)) {
      if (entered_len < (sizeof(entered) - 1)) {
        entered[entered_len++] = '1';
      }
    } else if (BTN_check_clear_pressed(2)) {
      if (entered_len < (sizeof(entered) - 1)) {
        entered[entered_len++] = '2';
      }
    } else if (BTN_check_clear_pressed(3)) {
      entered[entered_len] = '\0';
      
      if (strcmp(entered, password) == 0) {
        printk("Correct!\n");
      } else {
        printk("Incorrect!\n");
        printk("Entered: %s\n", entered);
      }

      entered_len = 0;
      entered[0] = '\0';
      waiting = true;
      LED_set(0, 0);
      LED_set(1, 0);
      LED_set(2, 0);
      LED_set(3, 0);
    }

    k_msleep(SLEEP_MS);
  }

  return 0;
}

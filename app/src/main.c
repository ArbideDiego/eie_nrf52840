/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// libraries to not show registers, only this, to check libraries ctrl+p
#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

int main(void) {
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  while (1) {
    k_msleep(SLEEP_MS);
    // give status a value each loop
    const char *status = "0"; 
    
    if (BTN_is_pressed(BTN0) == 1) { 
      status = "Button pressed";
    }
    
    printk("Status: %s\n", status);
  }
  return 0;
}
// LIBRARIES
#include "my_state_machine.h"

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

#include "BTN.h"
#include "LED.h"

#define SLEEP_MS 1

// MAIN
int main(void)
{
    // Setup buttons and leds/ validating not getting errrors
    if (0 > BTN_init()) {
        return 0;
    }
    if (0 > LED_init()) {
        return 0;
    }

    // Initialize the state machine
    state_machine_init();

    // Run the state machine in a continuous loop
    while (1) {
        state_machine_run();
        
        // Sleep for 10 ms. 
        k_msleep(10); 
    }

    return 0;
}
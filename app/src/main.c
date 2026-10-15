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
int main(void) {
    if (0 > BTN_init()){
        return 0;
    }
    if (0 > LED_init()){
        return 0;
    }

    state_machine_init();

    while(1){

        int ret = state_machine_run();
        if (0 > ret){
            return 0;
        }

        k_msleep(SLEEP_MS);
    }
    return 0;
}
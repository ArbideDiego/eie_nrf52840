// LIBRARIES
#include "my_state_machine.h"

#include <stdbool.h>
#include <zephyr/smf.h>

#include "BTN.h"
#include "LED.h"

/*
    -------------------------------------Function prototype----------------------------------------
    We need everyone of this, if there is noot action use NULL
    entry:every state entered
    run:every update
    exit:state exit
*/ 

static void state_0_entry(void *o);
static enum smf_state_result state_0_run(void *o);
static void state_0_exit(void *o);

static void state_1_entry(void *o);
static enum smf_state_result state_1_run(void *o);
static void state_1_exit(void *o);

static void state_2_entry(void *o);
static enum smf_state_result state_2_run(void *o);
static void state_2_exit(void *o);

static void state_3_entry(void *o);
static enum smf_state_result state_3_run(void *o);
static void state_3_exit(void *o);

static void state_4_entry(void *o);
static enum smf_state_result state_4_run(void *o);
static void state_4_exit(void *o);

/*
    -------------------------------------Type defs----------------------------------------
*/ 
enum sm_state {
    state_0,
    state_1,
    state_2,
    state_3,
    state_4
};

// zephyr uses a struct defined to keep track of the state machine
typedef struct my_sm_obj {
    // context variable used by zephyr
    struct smf_ctx ctx;

    uint16_t timer_counter;
    bool blink_state;

} my_sm_obj;


/*
    -------------------------------------Local Variables----------------------------------------
*/ 

static my_sm_obj sm_obj;

// map the functions with the states
static const struct smf_state states[] = {
    [state_0] = SMF_CREATE_STATE(state_0_entry, state_0_run, state_0_exit, NULL, NULL),
    [state_1] = SMF_CREATE_STATE(state_1_entry, state_1_run, state_1_exit, NULL, NULL),
    [state_2] = SMF_CREATE_STATE(state_2_entry, state_2_run, state_2_exit, NULL, NULL),
    [state_3] = SMF_CREATE_STATE(state_3_entry, state_3_run, state_3_exit, NULL, NULL),
    [state_4] = SMF_CREATE_STATE(state_4_entry, state_4_run, state_4_exit, NULL, NULL),
};

// logic fot the states

//state 0 
static void state_0_entry(void *o) {
    printk("Entered State S0\n");
    //Turn all LEDs OFF
      LED_set(0,0);
      LED_set(1,0);
      LED_set(2,0);
      LED_set(3,0);
}

static enum smf_state_result state_0_run(void *o) {
    // Check transitions
    if (BTN_is_pressed(0)) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_1]);
    } 
    // else if (BTN_is_pressed(3)) {
    //     smf_set_state(SMF_CTX(&sm_obj), &states[state_4]);
    // }
    return SMF_EVENT_HANDLED;
}

static void state_0_exit(void *o) {
    printk("Exited State S0\n");
}

//state 1
static void state_1_entry(void *o) {
    printk("Entered State S1\n");
    struct my_sm_obj *sm = (struct my_sm_obj *)o;
    sm->timer_counter = 0; // Reset counter for blinking /sm pointerr/ ->acces the variable
}

static enum smf_state_result state_1_run(void *o) {
    struct my_sm_obj *sm = (struct my_sm_obj *)o;
    sm->timer_counter++;

    // 4 Hz = 250ms period = 125ms per toggle. If 1 tick = 10ms, toggle every ~12 ticks.
    if (sm->timer_counter % 12 == 0) {
        sm->blink_state = !sm->blink_state;
        if (sm->blink_state) {   LED_set(0,1); } else {   LED_set(0,0); }
    }

    // Check transitions with btns
    if (BTN_is_pressed(1)) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_2]);
    } else if (BTN_is_pressed(2)) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_4]);
    } else if (BTN_is_pressed(3)) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_0]);
    }
    return SMF_EVENT_HANDLED;
}

static void state_1_exit(void *o) {
    printk("Exited State S1\n");
}

//state 2
static void state_2_entry(void *o) {
    printk("Entered State S2\n");
    struct my_sm_obj *sm = (struct my_sm_obj *)o;
    sm->timer_counter = 0; // Reset timer

    //Turn LED 1 and 3 ON. Turn LED 2 and 4 OFF.
      LED_set(3,0);
      LED_set(1,0);

      LED_set(0,1);
      LED_set(2,1);
}

static enum smf_state_result state_2_run(void *o) {
    struct my_sm_obj *sm = (struct my_sm_obj *)o;
    sm->timer_counter++;

    // Check transitions
    if (BTN_is_pressed(3)) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_0]);
    } 
    else if (sm->timer_counter >= 200) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_3]);
    }
    return SMF_EVENT_HANDLED;
}

static void state_2_exit(void *o) {
    printk("Exited State S2\n");
}

//state 3
static void state_3_entry(void *o) {
    printk("Entered State S3: LED 1&3 OFF, 2&4 ON\n");
    struct my_sm_obj *sm = (struct my_sm_obj *)o;
    sm->timer_counter = 0;
    
      LED_set(0,0);
      LED_set(1,1);
      LED_set(2,0);
      LED_set(3,1);
}

static enum smf_state_result state_3_run(void *o) {
    struct my_sm_obj *sm = (struct my_sm_obj *)o;
    sm->timer_counter++;

    if (BTN_is_pressed(3)) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_0]);
    } 
    // Wait 1 second (100 ticks at 10ms per tick)
    else if (sm->timer_counter >= 100) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_2]);
    }
    return SMF_EVENT_HANDLED;
}

static void state_3_exit(void *o) {
    printk("Exited State S3\n");
}

// state 4

static void state_4_entry(void *o) {
    printk("Entered State S4: All LEDs blink at 16 Hz\n");
    struct my_sm_obj *sm = (struct my_sm_obj *)o;
    sm->timer_counter = 0;
    sm->blink_state = 0;
}

static enum smf_state_result state_4_run(void *o) {
    struct my_sm_obj *sm = (struct my_sm_obj *)o;
    sm->timer_counter++;

    // 16 Hz = 62.5ms period = ~31ms per toggle. If 1 tick = 10ms, toggle every 3 ticks.
    if (sm->timer_counter % 3 == 0) {
        sm->blink_state = !sm->blink_state;
        if (sm->blink_state) {
              LED_set(0,1);   LED_set(1,1);   LED_set(2,1);   LED_set(3,1);
        } else {
              LED_set(0,0);   LED_set(1,0);   LED_set(2,0);   LED_set(3,0);
        }
    }

    if (BTN_is_pressed(3)) {
        smf_set_state(SMF_CTX(&sm_obj), &states[state_0]);
    }
    return SMF_EVENT_HANDLED;
}

static void state_4_exit(void *o) {
    printk("Exited State S4\n");
}

/* --- Initialization & Run --- */
void state_machine_init(void) {
    // Set initial state to S0
    smf_set_initial(SMF_CTX(&sm_obj), &states[state_0]);
}

void state_machine_run(void) {
    // Execute the current state
    smf_run_state(SMF_CTX(&sm_obj));
}
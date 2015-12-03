// All timer and led controls are put in controls.c

#ifndef _CONTROLS_H_
#define _CONTROLS_H_

// configurations of flashing multipliers:
extern const unsigned char SWITCH_OFF_FLASHING_MULTIPLIER;
extern const unsigned char SLOW_FLASHING_MULTIPLIER;
extern const unsigned char FAST_FLASHING_MULTIPLIER;
extern unsigned char flasher_cycle_multiplier[4];

/*	initially all flashers are turned off
	note that PORTA is externally pulled-up	*/
extern unsigned char previous_PINA;
extern unsigned char flasher_configuration[4];

// possible timer target states:
enum REQUIRED_TIMER_STATE {OFF, ON, AS_IT_IS};

// a utility check macro:
#define IS_BIT_SET(value, pos) (value & (1<<pos))

void switch_LED(unsigned char led_index, unsigned char required_state);

void update_timer(unsigned char flasher_index);

void apply_flasher_configuration(unsigned char flasher_index);

void switch_off_all_timers_and_LEDs();

#endif

/*
 * ATmega32TestBoard.c
 *
 * Created: 2014-12-18 20:27:54
 *  Author: amr ergawy
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

#include "intializations.h"
#include "controls.h"
#include "configurations.h"

unsigned char flasher_counter[4] = {0x00, 0x00, 0x00, 0x00};

ISR (TIMER0_COMP_vect) {
	flasher_counter[0] += 0x01;
	if (flasher_counter[0] == flasher_cycle_multiplier[0]) {
		if (IS_BIT_SET(PORTB, PB3)) {
			switch_LED(0, 0);
		} else {
			switch_LED(0, 1);
		}
		flasher_counter[0] = 0x00;
	}
}

ISR (TIMER1_COMPA_vect) {
	flasher_counter[1] += 0x01;
	if (flasher_counter[1] == flasher_cycle_multiplier[1]) {
		if (IS_BIT_SET(PORTD, PD5)) {
			switch_LED(1, 0);
		} else {
			switch_LED(1, 1);
		}
		flasher_counter[1] = 0x00;
	}
}

ISR (TIMER1_COMPB_vect) {
	flasher_counter[2] += 0x01;
	if (flasher_counter[2] == flasher_cycle_multiplier[2]) {
		if (IS_BIT_SET(PORTD, PD4)) {
			switch_LED(2, 0);
		} else {
			switch_LED(2, 1);
		}
		flasher_counter[2] = 0x00;
	}
}

ISR (TIMER2_COMP_vect) {
	flasher_counter[3] += 0x01;
	if (flasher_counter[3] == flasher_cycle_multiplier[3]) {
		if (IS_BIT_SET(PORTD, PD7)) {
			switch_LED(3, 0);
		} else {
			switch_LED(3, 1);
		}
		flasher_counter[3] = 0x00;
	}
}

void reset_all_flasher_counters(void) {
	for (int i=0; i<4; i++) {
		flasher_counter[i] = 0x00;
	}
}

// This is a test button, it switches the timers off and then switches all LEDs on.
ISR (INT0_vect) {
	// for protection and consistency, switch off every thing.
	switch_off_all_timers_and_LEDs();
	reset_all_flasher_counters();
	// switch all LEDs on.
	for (int i=0; i<4; i++) {
		switch_LED(i, 1);
	}
}

// This is an emergency button, switch off all timers and LEDs immediately
ISR (INT1_vect) {
	switch_off_all_timers_and_LEDs();
	reset_all_flasher_counters();
}

// reads flashers configurations and switch off all timers
ISR (INT2_vect) {
	
	// for protection and consistency, switch off every thing
	switch_off_all_timers_and_LEDs();
	reset_all_flasher_counters();

	// only when PINA changes, read LEDs configurations
	if ((previous_PINA ^ PINA) > 0) {
		previous_PINA = PINA;	// save PINA
		flasher_configuration[0] = 0x03 & previous_PINA;
		flasher_configuration[1] = (0x0C & previous_PINA) >> 2;
		flasher_configuration[2] = (0x30 & previous_PINA) >> 4;
		flasher_configuration[3] = (0xC0 & previous_PINA) >> 6;
	}

	// apply the new flashers configurations
	for (int i=0; i<4; i++) {
		apply_flasher_configuration(i);
	}
}

int main(void) {

	initiate_global_settings();

	initiate_timers_oscilators();
	
	initiate_io_ports();
	
	initiate_external_interrupts();

	while(1){}

	return 0;
}
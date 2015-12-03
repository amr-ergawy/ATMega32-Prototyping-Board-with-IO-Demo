#include "controls.h"

#include <avr/io.h>
#include <avr/interrupt.h>

// mapping from flashers to timers.
const unsigned char flasher_timer_map[4] = {0, 1, 1, 2};

void switch_LED(unsigned char led_index, unsigned char required_state){
	
	// switch the LED itself.
	switch(led_index){
		case 0:
			switch(required_state){
				case 0:
					PORTB &= ~(_BV(PB3));
					break;
				case 1:
					PORTB |= _BV(PB3);
					break;
				default:
					// do no thing.
					break;
			}
			break;
		case 1:
			switch(required_state){
				case 0:
					PORTD &= ~(_BV(PD5));
					break;
				case 1:
					PORTD |= _BV(PD5);
					break;
				default:
					// do no thing.
					break;
			}
			break;
		case 2:
			switch(required_state){
				case 0:
					PORTD &= ~(_BV(PD4));
					break;
				case 1:
					PORTD |= _BV(PD4);
					break;
				default:
					// do no thing.
					break;
			}
			break;
		case 3:
			switch(required_state){
				case 0:
					PORTD &= ~(_BV(PD7));
					break;
				case 1:
					PORTD |= _BV(PD7);
					break;
				default:
					// do no thing.
					break;
			}
			break;
		default:
			// do no thing.
			break;
	}
}

void update_timer(unsigned char flasher_index){
	
	// update the timer state.
	enum REQUIRED_TIMER_STATE required_timer_state = AS_IT_IS;

	// update the required interrupts.
	cli();
	switch(flasher_index){
		case 0:
			if(flasher_cycle_multiplier[flasher_index] ==
				SWITCH_OFF_FLASHING_MULTIPLIER){
				TIMSK &= ~(_BV(OCIE0));
				required_timer_state = OFF;
			} else {
				TIMSK |= _BV(OCIE0);
				required_timer_state = ON;
			}
			break;
		case 1:
			if(flasher_cycle_multiplier[flasher_index] ==
				SWITCH_OFF_FLASHING_MULTIPLIER){
				TIMSK &= ~(_BV(OCIE1A));
				if(!IS_BIT_SET(TIMSK, OCIE1B)){
					required_timer_state = OFF;
				}
			} else {
				TIMSK |= _BV(OCIE1A);
				required_timer_state = ON;				
			}
			break;
		case 2:
			if(flasher_cycle_multiplier[flasher_index] ==
				SWITCH_OFF_FLASHING_MULTIPLIER){
				TIMSK &= ~(_BV(OCIE1B));
				if(!IS_BIT_SET(TIMSK, OCIE1A)){
					required_timer_state = OFF;
				}
			} else {
				TIMSK |= _BV(OCIE1B);
				required_timer_state = ON;				
			}
			break;
		case 3:
			if(flasher_cycle_multiplier[flasher_index] ==
				SWITCH_OFF_FLASHING_MULTIPLIER){
				TIMSK &= ~(_BV(OCIE2));
				required_timer_state = OFF;
			} else {
				TIMSK |= _BV(OCIE2);
				required_timer_state = ON;
			}
			break;
		default:
			break;
	}
	sei();
		
	unsigned char timer_index = flasher_timer_map[flasher_index];
	switch(timer_index){
		case 0:
			TCNT0 = 0x00; 	// reset the counter.
			if(required_timer_state == OFF){
				TCCR0 &= ~(_BV(CS02) | _BV(CS01) | _BV(CS00));
			} else if(required_timer_state == ON){
				// using 1024 prescaler.
				TCCR0 |= _BV(CS02) | _BV(CS00);
				TCCR0 &= ~(_BV(CS01));	
			} else {
				// as it is.
			}
			break;
		case 1:
			TCNT1 = 0x00;	// reset the counter.
			if(required_timer_state == OFF){
				TCCR1B &= ~(_BV(CS12) | _BV(CS11) | _BV(CS10));
			} else if (required_timer_state == ON){
				// using 1024 prescaler.
				TCCR1B |= _BV(CS12) | _BV(CS10);
				TCCR1B &= ~(_BV(CS11));
			} else {
				// as it is.
			}
			break;
		case 2:
			TCNT2 = 0x00; // reset the counter.
			if(required_timer_state == OFF){
				TCCR2 &= ~(_BV(CS22) | _BV(CS21) | _BV(CS20));
			} else if (required_timer_state == ON){
				// using prescler 1024.
				TCCR2 |= _BV(CS22) | _BV(CS21) | _BV(CS20);
			} else {
				// as it is.
			}
			break;
		default:
			// do no thing.
			break;
	}
}

void apply_flasher_configuration(unsigned char flasher_index){
	
	// note that all pins of PORTA are externally pulled up.
	if(IS_BIT_SET(flasher_configuration[flasher_index], 0)){
		flasher_cycle_multiplier[flasher_index] = 
			SWITCH_OFF_FLASHING_MULTIPLIER;
	} else {
		if(IS_BIT_SET(flasher_configuration[flasher_index], 1)){
			flasher_cycle_multiplier[flasher_index] =
				SLOW_FLASHING_MULTIPLIER;
		} else {
			flasher_cycle_multiplier[flasher_index] = 
				FAST_FLASHING_MULTIPLIER;
		}
	}

	// update the corresponding timer.
	update_timer(flasher_index);
}

void switch_off_all_timers_and_LEDs(){
	
	int i;
	for(i=0; i<4; i++){
		// note that all POTRA pins are externally pulled up.
		flasher_configuration[i] = 0x03;
		apply_flasher_configuration(i);
		switch_LED(i, 0);
	}
	previous_PINA = 0xFF;
}
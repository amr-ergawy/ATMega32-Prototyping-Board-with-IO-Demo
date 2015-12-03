#include "intializations.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "controls.h"

void initiate_global_settings(void){

	// interrupts at Flash start, ATMega32 data-sheet, page 49.
	GICR |= _BV(IVCE);
	GICR &= ~(_BV(IVSEL));
	
	// disable watch-dog timer, ATMega32 data-sheet, page 44.
	cli();
	WDTCR |= _BV(WDTOE) | _BV(WDE);
	WDTCR = 0x00;
	sei();

	// disable ADC, ATMega32 data-sheet, page 224.
	ADCSRA &= ~(_BV(ADEN));

	// disable the analog comparator, ATMega32 data-sheet, page 206.
	ACSR |= _BV(ACD);
}

void initiate_timers_oscilators(void){

	// for all timers, use CTC mode.
	TCCR0 |= _BV(WGM01);
	TCCR0 &= _BV(WGM00);

	TCCR2 |= _BV(WGM21);
	TCCR2 &= _BV(WGM20);

	TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));
	TCCR1B |= _BV(WGM12);
	TCCR1B &= ~(_BV(WGM13));

	// for all timers, use compare-match every 0.524288 seconds.
	OCR0 = 0xFF;
	
	OCR1AH = 0x00;
	OCR1AL = 0xFF;
	OCR1BH = 0x00;
	OCR1BL = 0xFF;

	OCR2 = 0xFF;

	// for all timers, disconnect OCn.
	TCCR0 &= ~(_BV(COM01) | _BV(COM00));
	TCCR1A &= ~(_BV(COM1A1) | _BV(COM1A0) |
			    _BV(COM1B1) | _BV(COM1B0));
	TCCR2 &= ~(_BV(COM21) | _BV(COM20));

	// apply the initial flasher cycle multiplier timers.
	int i;
	for(i=0; i<4; i++){
		update_timer(i);
	}
}

void initiate_io_ports(void){

	// enable pull-up resistors.
	SFIOR &= ~(_BV(PUD));

	// configure PORTA:
	DDRA = 0x00;	// all pins are inputs.

	// configure PORTB:
	DDRB = _BV(DDB3);	// all pins are inputs except pin 3.
	PORTB = 0xF3;		// set pin 3 to low, recovering from "reset tri-state"
						// enable internal pull-ups for 0-1, and 4-7.

	// configure PORTC:
	DDRC = 0x00;	// all pins are inputs.
	PORTC = 0xFF;	// enable internal pull-ups for all.

	// configure PORTD:
	DDRD = _BV(DDD4) | _BV(DDD5) | _BV(DDD7);	// all pins are inputs except pins 4 and 5.
	PORTD = _BV(PD0) | _BV(PD1) | _BV(PD6);		// enable internal pull-ups for pins 0-1, and 6.
}

void initiate_external_interrupts(void){

	// this procedure is recommended in ATMega32 datasheet, section 13.1.2
	// if it is not followed, at least INT2 occurs while setting
	// then, the program counter jumps searching for INT2_vect
	// INT1 and INT2 are enabled as rising edge based
	// INT0 is enabled at any logic change.
	
	cli();

	GICR &= ~(_BV(INT0)) | ~(_BV(INT1)) | ~(_BV(INT2));
	
	MCUCR |= _BV(ISC11) | _BV(ISC10) | _BV(ISC00);
	MCUSR &= ~(_BV(ISC01));
	MCUCSR |= _BV(ISC2); 
	
	GIFR &= ~(_BV(INTF0)) | ~(_BV(INTF1)) | ~(_BV(INTF2));
	
	GICR |= _BV(INT0) | _BV(INT1) | _BV(INT2);
	
	sei();	
}

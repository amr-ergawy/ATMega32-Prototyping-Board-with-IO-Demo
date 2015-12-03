#ifndef _CONFIGURATIONS_H_
#define _CONFIGURATIONS_H_

/*	initially all flashers are turned off
	note that PORTA is externally pulled-up	*/
unsigned char previous_PINA = 0xFF;
unsigned char flasher_configuration[4] = {0x03, 0x03, 0x03, 0x03};

// configurations of flashing multipliers:
const unsigned char SWITCH_OFF_FLASHING_MULTIPLIER = 0;
const unsigned char SLOW_FLASHING_MULTIPLIER = 2;
const unsigned char FAST_FLASHING_MULTIPLIER = 1;
unsigned char flasher_cycle_multiplier[4] = {0, 0, 0, 0};

#endif

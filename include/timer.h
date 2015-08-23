#ifndef timer_h
#define timer_h

#ifndef ___AVR_ATMEGA328P___
	#error "The timer module is not available on the chosen MCU."
#endif

#include <avr/io.h>

uint8_t timer_has_elapsed()
{
	// check for the counter overflow flag
	return TIFR0 & _BV(TOV0);
}
void timer_reset()
{
	// clear the counter overflow flag
	TIFR0 |= _BV(TOV0);
}
void timer_restart()
{
	// reset the counter
	TCNT0 = 0x00;
}
uint8_t timer_value()
{
	// get the counter value
	return TCNT0;
}

void timer_initialize()
{
	// disable compare output mode (COM0A = 0b00, COM0B = 0b00)
	// disable waveform generation mode (WGM0 = 0b000)
	// set timer to increment every 64 clock cycles, causes overflow every 1 ms at 16 MHz (CS0 = 0b011)
	TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
	TCCR0B = (0 << WGM02) | (0 << CS02) | (1 << CS01) | (1 << CS00);

	// clear the counter overflow flag
	TIFR0 |= _BV(TOV0);
	// reset the counter
	TCNT0 = 0x00;
}
void timer_dispose()
{
	// reset control registers to their initial values
	TCCR0A = 0x00;
	TCCR0B = 0x00;

	// clear the counter overflow flag
	TIFR0 |= _BV(TOV0);
	// reset the counter
	TCNT0 = 0x00;
}

#endif

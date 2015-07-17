#ifndef pwm_h
#define pwm_h

#include <avr/io.h>

void pwm_initialize()
{
	// set compare output mode to clear on compare match, set at bottom (COM0A = 0b10, COM0B = 0b10)
	// set waveform generation mode to phase correct PWM with TOP = MAX (WGM0 = 0b001)
	// set timer to increment every clock cycle, causes ~ 31.4 kHz PWM signal at 16 MHz (CS0 = 0b001)
	TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (1 << WGM00);
	TCCR0B = (0 << WGM02) | (0 << CS02) | (0 << CS01) | (1 << CS00);
	// disable all interrupts (OCIE0B = 0b0, OCIE0A = 0b0, TOIE0 = 0b0)
	TIMSK0 = (0 << OCIE0B) | (0 << OCIE0A) | (0 << TOIE0);

	// set PWM values to 0 % duty
	OCR0A = 0x00;
	OCR0B = 0x00;

	// clear the counter overflow flag
	TIFR0 |= _BV(TOV0);
	// reset the counter
	TCNT0 = 0x00;

	// enable output on OCR0A and OCR0B pins
	DDRD |= _BV(5) | _BV(6);
}
void pwm_dispose() { }

void pwm_set_value(uint8_t index, uint8_t value)
{
	switch (index)
	{
		case 0: OCR0A = value; break;
		case 1: OCR0B = value; break;
	}
}

#endif

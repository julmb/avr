#ifndef pwm_h
#define pwm_h

#ifndef ___AVR_ATMEGA328P___
	#error "The PWM module is not available on the chosen MCU."
#endif

#include <avr/io.h>

uint8_t pwm_get_frequency_bits(uint8_t frequency)
{
	switch (frequency)
	{
		// 1: 31.373 kHz at 16 MHz clock
		case 0: return 0b001;
		// 8: 3.9216 kHz at 16 MHz clock
		case 1: return 0b010;
		// 64: 490.20 Hz at 16 MHz clock
		case 2: return 0b011;
		// 256: 122.55 Hz at 16 MHz clock
		case 3: return 0b100;
		// 1024: 30.637 Hz at 16 MHz clock
		case 4: return 0b101;
	}

	return 0b000;
}

void pwm_initialize(uint8_t frequency)
{
	uint8_t frequency_bits = pwm_get_frequency_bits(frequency);

	// set compare output mode to clear on compare match, set at bottom (COM0A = 0b10, COM0B = 0b10)
	// set waveform generation mode to phase correct PWM with TOP = 0xFF (WGM0 = 0b001)
	TCCR0A = (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (1 << WGM00);
	TCCR0B = (0 << WGM02) | frequency_bits;

	// set compare output mode to clear on compare match, set at bottom (COM1A = 0b10, COM1B = 0b10)
	// set waveform generation mode to phase correct PWM with TOP = 0x00FF (WGM1 = 0b0001)
	TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (1 << WGM10);
	TCCR1B = (0 << WGM13) | (0 << WGM12) | frequency_bits;

	// set compare output mode to clear on compare match, set at bottom (COM2A = 0b10, COM2B = 0b10)
	// set waveform generation mode to phase correct PWM with TOP = 0xFF (WGM2 = 0b001)
	TCCR2A = (1 << COM2A1) | (0 << COM2A0) | (1 << COM2B1) | (0 << COM2B0) | (0 << WGM21) | (1 << WGM20);
	TCCR2B = (0 << WGM22) | frequency_bits;

	// set PWM values to 0 % duty
	OCR0A = 0x00;
	OCR0B = 0x00;
	OCR1A = 0x0000;
	OCR1B = 0x0000;
	OCR2A = 0x00;
	OCR2B = 0x00;

	// reset the counters
	TCNT0 = 0x00;
	TCNT1 = 0x0000;
	TCNT2 = 0x00;

	// enable output on the OC0A and OC0B pins
	DDRD |= _BV(5);
	DDRD |= _BV(6);

	// enable output on the OC1A and OC1B pins
	DDRB |= _BV(1);
	DDRB |= _BV(2);

	// enable output on the OC2A and OC2B pins
	DDRB |= _BV(3);
	DDRD |= _BV(3);
}
void pwm_dispose()
{
	// reset control registers for timer 0 to their initial values
	TCCR0A = 0x00;
	TCCR0B = 0x00;

	// reset control registers for timer 1 to their initial values
	TCCR1A = 0x00;
	TCCR1B = 0x00;

	// reset control registers for timer 2 to their initial values
	TCCR2A = 0x00;
	TCCR2B = 0x00;

	// reset the output compare registers
	OCR0A = 0x00;
	OCR0B = 0x00;
	OCR1A = 0x0000;
	OCR1B = 0x0000;
	OCR2A = 0x00;
	OCR2B = 0x00;

	// reset the counters
	TCNT0 = 0x00;
	TCNT1 = 0x0000;
	TCNT2 = 0x00;

	// disable output on the OC0A and OC0B pins
	DDRD &= ~ _BV(5);
	DDRD &= ~ _BV(6);

	// disable output on the OC1A and OC1B pins
	DDRB &= ~ _BV(1);
	DDRB &= ~ _BV(2);

	// disable output on the OC2A and OC2B pins
	DDRB &= ~ _BV(3);
	DDRD &= ~ _BV(3);
}

void pwm_set_value(uint8_t index, uint8_t value)
{
	switch (index)
	{
		case 0: OCR0A = value; break;
		case 1: OCR0B = value; break;
		case 2: OCR1AL = value; break;
		case 3: OCR1BL = value; break;
		case 4: OCR2A = value; break;
		case 5: OCR2B = value; break;
	}
}

#endif

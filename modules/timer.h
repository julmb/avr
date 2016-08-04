#ifndef timer_h
#define timer_h

#ifndef __AVR_ATmega328P__
	#error "The timer module is not available on the chosen MCU."
#endif

// TODO: are these still needed?
#include <stddef.h>
#include <stdint.h>

#include <avr/io.h>

typedef enum { divider_1, divider_8, divider_64, divider_256, divider_1024 } divider;

inline uint8_t timer_0_divider_bits(divider divider)
{
	switch (divider)
	{
		case divider_1: return (0 << CS02) | (0 << CS01) | (1 << CS00);
		case divider_8: return (0 << CS02) | (1 << CS01) | (0 << CS00);
		case divider_64: return (0 << CS02) | (1 << CS01) | (1 << CS00);
		case divider_256: return (1 << CS02) | (0 << CS01) | (0 << CS00);
		case divider_1024: return (1 << CS02) | (0 << CS01) | (1 << CS00);
		default: return 0;
	}
}
inline uint8_t timer_1_divider_bits(divider divider)
{
	switch (divider)
	{
		case divider_1: return (0 << CS12) | (0 << CS11) | (1 << CS10);
		case divider_8: return (0 << CS12) | (1 << CS11) | (0 << CS10);
		case divider_64: return (0 << CS12) | (1 << CS11) | (1 << CS10);
		case divider_256: return (1 << CS12) | (0 << CS11) | (0 << CS10);
		case divider_1024: return (1 << CS12) | (0 << CS11) | (1 << CS10);
		default: return 0;
	}
}
inline uint8_t timer_2_divider_bits(divider divider)
{
	switch (divider)
	{
		case divider_1: return (0 << CS22) | (0 << CS21) | (1 << CS20);
		case divider_8: return (0 << CS22) | (1 << CS21) | (0 << CS20);
		case divider_64: return (0 << CS22) | (1 << CS21) | (1 << CS20);
		case divider_256: return (1 << CS22) | (0 << CS21) | (0 << CS20);
		case divider_1024: return (1 << CS22) | (0 << CS21) | (1 << CS20);
		default: return 0;
	}
}

inline uint8_t timer_0_get() { return TCNT0; }
inline void timer_0_set(uint8_t value) { TCNT0 = value; }
inline uint8_t timer_0_overflown() { return TIFR0 & _BV(TOV0); }
inline void timer_0_reset() { TIFR0 |= _BV(TOV0); }

inline void timer_0_initialize(divider divider)
{
	// disable compare output (COM0A = 0b00, COM0B = 0b00)
	// disable waveform generation (WGM0 = 0b000)
	// set divider accordingly
	TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
	TCCR0B = (0 << WGM02) | timer_0_divider_bits(divider);

	timer_0_set(0x00);
	timer_0_reset();
}
inline void timer_0_dispose()
{
	// reset control registers to their initial values
	TCCR0A = 0x00;
	TCCR0B = 0x00;

	timer_0_set(0x00);
	timer_0_reset();
}

inline uint16_t timer_1_get() { return TCNT1; }
inline void timer_1_set(uint16_t value) { TCNT1 = value; }
inline uint8_t timer_1_overflown() { return TIFR1 & _BV(TOV1); }
inline void timer_1_reset() { TIFR1 |= _BV(TOV1); }

inline void timer_1_initialize(divider divider)
{
	// disable compare output (COM1A = 0b00, COM1B = 0b00)
	// disable waveform generation (WGM1 = 0b0000)
	// set divider accordingly
	TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10);
	TCCR1B = (0 << WGM13) | (0 << WGM12) | timer_1_divider_bits(divider);

	timer_1_set(0x0000);
	timer_1_reset();
}
inline void timer_1_dispose()
{
	// reset control registers to their initial values
	TCCR1A = 0x00;
	TCCR1B = 0x00;

	timer_1_set(0x0000);
	timer_1_reset();
}

inline uint8_t timer_2_get() { return TCNT2; }
inline void timer_2_set(uint8_t value) { TCNT2 = value; }
inline uint8_t timer_2_overflown() { return TIFR2 & _BV(TOV2); }
inline void timer_2_reset() { TIFR2 |= _BV(TOV2); }

inline void timer_2_initialize(divider divider)
{
	// disable compare output (COM2A = 0b00, COM2B = 0b00)
	// disable waveform generation (WGM2 = 0b000)
	// set divider accordingly
	TCCR2A = (0 << COM2A1) | (0 << COM2A0) | (0 << COM2B1) | (0 << COM2B0) | (0 << WGM21) | (0 << WGM20);
	TCCR2B = (0 << WGM22) | timer_2_divider_bits(divider);

	timer_2_set(0x00);
	timer_2_reset();
}
inline void timer_2_dispose()
{
	// reset control registers to their initial values
	TCCR2A = 0x00;
	TCCR2B = 0x00;

	timer_2_set(0x00);
	timer_2_reset();
}

#endif

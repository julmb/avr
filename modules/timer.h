#ifndef timer_h
#define timer_h

#ifndef __AVR_ATmega328P__
	#error "The timer module is not available on the chosen MCU."
#endif

// TODO: are these still needed?
#include <stddef.h>
#include <stdint.h>

#include <avr/io.h>

// TODO: split counter and timer?

typedef enum { divider_1, divider_8, divider_64, divider_256, divider_1024 } divider;

inline uint8_t divider_bits(uint8_t index, divider divider)
{
	switch (index)
	{
		case 0:
			switch (divider)
			{
				case divider_1: return (0 << CS02) | (0 << CS01) | (1 << CS00);
				case divider_8: return (0 << CS02) | (1 << CS01) | (0 << CS00);
				case divider_64: return (0 << CS02) | (1 << CS01) | (1 << CS00);
				case divider_256: return (1 << CS02) | (0 << CS01) | (0 << CS00);
				case divider_1024: return (1 << CS02) | (0 << CS01) | (1 << CS00);
				default: return 0;
			}
		case 1:
			switch (divider)
			{
				case divider_1: return (0 << CS12) | (0 << CS11) | (1 << CS10);
				case divider_8: return (0 << CS12) | (1 << CS11) | (0 << CS10);
				case divider_64: return (0 << CS12) | (1 << CS11) | (1 << CS10);
				case divider_256: return (1 << CS12) | (0 << CS11) | (0 << CS10);
				case divider_1024: return (1 << CS12) | (0 << CS11) | (1 << CS10);
				default: return 0;
			}
		case 2:
			switch (divider)
			{
				case divider_1: return (0 << CS22) | (0 << CS21) | (1 << CS20);
				case divider_8: return (0 << CS22) | (1 << CS21) | (0 << CS20);
				case divider_64: return (0 << CS22) | (1 << CS21) | (1 << CS20);
				case divider_256: return (1 << CS22) | (0 << CS21) | (0 << CS20);
				case divider_1024: return (1 << CS22) | (0 << CS21) | (1 << CS20);
				default: return 0;
			}
		default: return 0;
	}
}

typedef struct
{
	uint8_t index;
}
counter;

inline uint16_t counter_get(counter counter)
{
	switch (counter.index)
	{
		case 0: return TCNT0;
		case 1: return TCNT1;
		case 2: return TCNT2;
		default: return 0;
	}
}
inline void counter_set(counter counter, uint16_t value)
{
	switch (counter.index)
	{
		case 0: TCNT0 = value; break;
		case 1: TCNT1 = value; break;
		case 2: TCNT2 = value; break;
	}
}
inline uint8_t counter_overflown(counter counter)
{
	switch (counter.index)
	{
		case 0: return TIFR0 & _BV(TOV0);
		case 1: return TIFR1 & _BV(TOV1);
		case 2: return TIFR2 & _BV(TOV2);
		default: return 0;
	}
}
inline void counter_reset(counter counter)
{
	switch (counter.index)
	{
		case 0: TIFR0 |= _BV(TOV0); break;
		case 1: TIFR1 |= _BV(TOV1); break;
		case 2: TIFR2 |= _BV(TOV2); break;
	}
}

inline counter counter_initialize(uint8_t index, divider divider)
{
	counter counter =
	{
		.index = index
	};

	switch (counter.index)
	{
		case 0:
			// disable compare output (COM0A = 0b00, COM0B = 0b00)
			// disable waveform generation (WGM0 = 0b000)
			// set divider accordingly
			TCCR0A = (0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (0 << WGM01) | (0 << WGM00);
			TCCR0B = (0 << WGM02) | divider_bits(index, divider);
			break;
		case 1:
			// disable compare output (COM1A = 0b00, COM1B = 0b00)
			// disable waveform generation (WGM1 = 0b0000)
			// set divider accordingly
			TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10);
			TCCR1B = (0 << WGM13) | (0 << WGM12) | divider_bits(index, divider);
			break;
		case 2:
			// disable compare output (COM2A = 0b00, COM2B = 0b00)
			// disable waveform generation (WGM2 = 0b000)
			// set divider accordingly
			TCCR2A = (0 << COM2A1) | (0 << COM2A0) | (0 << COM2B1) | (0 << COM2B0) | (0 << WGM21) | (0 << WGM20);
			TCCR2B = (0 << WGM22) | divider_bits(index, divider);
			break;
	}

	counter_set(counter, 0x0000);
	counter_reset(counter);

	return counter;
}
inline void counter_dispose(counter counter)
{
	switch (counter.index)
	{
		case 0:
			// reset control registers to their initial values
			TCCR0A = 0x00;
			TCCR0B = 0x00;
			break;
		case 1:
			// reset control registers to their initial values
			TCCR1A = 0x00;
			TCCR1B = 0x00;
			break;
		case 2:
			// reset control registers to their initial values
			TCCR2A = 0x00;
			TCCR2B = 0x00;
			break;
	}

	counter_set(counter, 0x0000);
	counter_reset(counter);
}

typedef enum { comparer_a, comparer_b } comparer;

typedef struct
{
	counter counter;
	comparer comparer;
}
timer;

inline uint16_t timer_get(timer timer)
{
	switch (timer.counter.index)
	{
		case 0:
			switch (timer.comparer)
			{
				case comparer_a: return OCR0A;
				case comparer_b: return OCR0B;
				default: return 0;
			}
		case 1:
			switch (timer.comparer)
			{
				case comparer_a: return OCR1A;
				case comparer_b: return OCR1B;
				default: return 0;
			}
		case 2:
			switch (timer.comparer)
			{
				case comparer_a: return OCR2A;
				case comparer_b: return OCR2B;
				default: return 0;
			}
		default: return 0;
	}
}
inline void timer_set(timer timer, uint8_t value)
{
	switch (timer.counter.index)
	{
		case 0:
			switch (timer.comparer)
			{
				case comparer_a: OCR0A = value; break;
				case comparer_b: OCR0B = value; break;
			}
			break;
		case 1:
			switch (timer.comparer)
			{
				case comparer_a: OCR1A = value; break;
				case comparer_b: OCR1B = value; break;
			}
			break;
		case 2:
			switch (timer.comparer)
			{
				case comparer_a: OCR2A = value; break;
				case comparer_b: OCR2B = value; break;
			}
			break;
	}
}
inline uint8_t timer_elapsed(timer timer)
{
	switch (timer.counter.index)
	{
		case 0:
			switch (timer.comparer)
			{
				case comparer_a: return TIFR0 & _BV(OCF0A);
				case comparer_b: return TIFR0 & _BV(OCF0B);
				default: return 0;
			}
		case 1:
			switch (timer.comparer)
			{
				case comparer_a: return TIFR1 & _BV(OCF1A);
				case comparer_b: return TIFR1 & _BV(OCF1B);
				default: return 0;
			}
		case 2:
			switch (timer.comparer)
			{
				case comparer_a: return TIFR2 & _BV(OCF2A);
				case comparer_b: return TIFR2 & _BV(OCF2B);
				default: return 0;
			}
		default: return 0;
	}
}
inline void timer_reset(timer timer)
{
	switch (timer.counter.index)
	{
		case 0:
			switch (timer.comparer)
			{
				case comparer_a: TIFR0 |= _BV(OCF0A); break;
				case comparer_b: TIFR0 |= _BV(OCF0B); break;
			}
			break;
		case 1:
			switch (timer.comparer)
			{
				case comparer_a: TIFR1 |= _BV(OCF1A); break;
				case comparer_b: TIFR1 |= _BV(OCF1B); break;
			}
			break;
		case 2:
			switch (timer.comparer)
			{
				case comparer_a: TIFR2 |= _BV(OCF2A); break;
				case comparer_b: TIFR2 |= _BV(OCF2B); break;
			}
			break;
	}
}

inline timer timer_initialize(counter counter, comparer comparer)
{
	timer timer =
	{
		.counter = counter,
		.comparer = comparer
	};

	timer_set(timer, 0x0000);
	timer_reset(timer);

	return timer;
}
inline void timer_dispose(timer timer)
{
	timer_set(timer, 0x0000);
	timer_reset(timer);
}

#endif

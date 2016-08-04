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

typedef struct
{
	uint8_t index;
}
counter;

inline uint8_t counter_get(counter counter)
{
	switch (counter.index)
	{
		case 0: return TCNT0;
		default: return 0;
	}
}
inline void counter_set(counter counter, uint8_t value)
{
	switch (counter.index)
	{
		case 0: TCNT0 = value; break;
	}
}
inline uint8_t counter_overflown(counter counter)
{
	switch (counter.index)
	{
		case 0: return TIFR0 & _BV(TOV0);
		default: return 0;
	}
}
inline void counter_reset(counter counter)
{
	switch (counter.index)
	{
		case 0: TIFR0 |= _BV(TOV0); break;
	}
}

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
		default: return 0;
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
	}

	counter_reset(counter);
	counter_set(counter, 0x00);

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
	}

	counter_reset(counter);
	counter_set(counter, 0x00);
}

typedef enum { comparer_a, comparer_b } comparer;

typedef struct
{
	counter counter;
	comparer comparer;
}
timer;

inline uint8_t timer_get(timer timer)
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
				case comparer_a: return OCR0A = value; break;
				case comparer_b: return OCR0B = value; break;
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
				case comparer_a: return TIFR0 |= _BV(OCF0A); break;
				case comparer_b: return TIFR0 |= _BV(OCF0B); break;
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

	timer_reset(timer);
	timer_set(timer, 0x00);

	return timer;
}
inline void timer_dispose(timer timer)
{
	timer_reset(timer);
	timer_set(timer, 0x00);
}

#endif

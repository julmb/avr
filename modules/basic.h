#ifndef basic_h
#define basic_h

#include <stddef.h>
#include <stdint.h>

#include <avr/wdt.h>

typedef enum { unknown, power, external, software, brown_out } reset_type;

reset_type initialize()
{
	uint8_t status = MCUSR;

	MCUSR = 0x00;

	wdt_disable();

	if (status == _BV(PORF)) return power;
	if (status == _BV(EXTRF)) return external;
	if (status == _BV(WDRF)) return software;
	if (status == _BV(BORF)) return brown_out;

	return unknown;
}

void reset()
{
	wdt_enable(WDTO_15MS);

	while (1);
}

void start()
{
	asm("jmp 0");
}

#endif

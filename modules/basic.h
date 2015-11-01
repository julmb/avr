#ifndef basic_h
#define basic_h

#include <stddef.h>
#include <stdint.h>

#include <avr/wdt.h>

#define FLASH_BASE ((void*)0)
#define FLASH_END ((void*)FLASHEND + 1)
#define FLASH_LENGTH (FLASH_END - FLASH_BASE)
#define FLASH_PAGE_LENGTH ((size_t)SPM_PAGESIZE)
#define FLASH_PAGE_COUNT (FLASH_LENGTH / FLASH_PAGE_LENGTH)

#define EEPROM_BASE ((void*)0)
#define EEPROM_END ((void*)E2END + 1)
#define EEPROM_LENGTH (EEPROM_END - EEPROM_BASE)
#define EEPROM_PAGE_LENGTH ((size_t)E2PAGESIZE)
#define EEPROM_PAGE_COUNT (EEPROM_LENGTH / EEPROM_PAGE_LENGTH)

#define SRAM_BASE ((void*)RAMSTART)
#define SRAM_END ((void*)RAMEND + 1)
#define SRAM_LENGTH (SRAM_END - SRAM_BASE)

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

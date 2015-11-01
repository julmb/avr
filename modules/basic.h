#ifndef basic_h
#define basic_h

#include <stddef.h>
#include <stdint.h>

#include <avr/wdt.h>

#define FLASH_BASE ((size_t)0)
#define FLASH_SIZE (((size_t)FLASHEND + 1) - FLASH_BASE)
#define FLASH_PAGE_SIZE ((size_t)SPM_PAGESIZE)
#define FLASH_PAGE_COUNT (FLASH_SIZE / FLASH_PAGE_SIZE)

#define EEPROM_BASE ((size_t)0)
#define EEPROM_SIZE (((size_t)E2END + 1) - EEPROM_BASE)
#define EEPROM_PAGE_SIZE ((size_t)E2PAGESIZE)
#define EEPROM_PAGE_COUNT (EEPROM_SIZE / EEPROM_PAGE_SIZE)

#define SRAM_BASE ((size_t)RAMSTART)
#define SRAM_SIZE (((size_t)RAMEND + 1) - SRAM_BASE)

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

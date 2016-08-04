#ifndef memory_h
#define memory_h

#include <stddef.h>
#include <stdint.h>

#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/eeprom.h>

#include "basic.h"

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

#define SIGCAL_BASE ((void*)0)
#define SIGCAL_END ((void*)0x10)
#define SIGCAL_LENGTH (SIGCAL_END - SIGCAL_BASE)
#define SIGCAL_PAGE_LENGTH ((size_t)1)
#define SIGCAL_PAGE_COUNT (SIGCAL_LENGTH / SIGCAL_PAGE_LENGTH)

#define FUSELOCK_BASE ((void*)0)
#define FUSELOCK_END ((void*)0x10)
#define FUSELOCK_LENGTH (FUSELOCK_END - FUSELOCK_BASE)
#define FUSELOCK_PAGE_LENGTH ((size_t)1)
#define FUSELOCK_PAGE_COUNT (FUSELOCK_LENGTH / FUSELOCK_PAGE_LENGTH)

#define SRAM_BASE ((void*)RAMSTART)
#define SRAM_END ((void*)RAMEND + 1)
#define SRAM_LENGTH (SRAM_END - SRAM_BASE)

void flash_read_page(uint8_t page_index, void* data)
{
	void* page_position = FLASH_BASE + page_index * FLASH_PAGE_LENGTH;

	uint8_t* bytes = data;
	for (uint8_t byte_index = 0; byte_index < FLASH_PAGE_LENGTH; byte_index++)
		*bytes++ = pgm_read_byte(page_position + byte_index);
}
void flash_write_page(uint8_t page_index, void* data)
{
	void* page_position = FLASH_BASE + page_index * FLASH_PAGE_LENGTH;

	boot_page_erase(page_position);
	boot_spm_busy_wait();

	uint8_t* bytes = data;
	for (uint8_t byte_index = 0; byte_index < FLASH_PAGE_LENGTH; byte_index += 2)
		boot_page_fill(byte_index, *bytes++ << 0 | *bytes++ << 8);

	boot_page_write(page_position);
	boot_spm_busy_wait();

	boot_rww_enable();
}

void eeprom_read_page(uint8_t page_index, void* data)
{
	eeprom_read_block(data, EEPROM_BASE + page_index * EEPROM_PAGE_LENGTH, EEPROM_PAGE_LENGTH);
}
void eeprom_write_page(uint8_t page_index, void* data)
{
	eeprom_update_block(data, EEPROM_BASE + page_index * EEPROM_PAGE_LENGTH, EEPROM_PAGE_LENGTH);
}

void sigcal_read_page(uint8_t page_index, void* data)
{
	uint8_t* bytes = data;
	*bytes = boot_signature_byte_get(page_index);
}

void fuselock_read_page(uint8_t page_index, void* data)
{
	uint8_t* bytes = data;
	*bytes = boot_lock_fuse_bits_get(page_index);
}

#endif

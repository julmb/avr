#ifndef eeprom_h
#define eeprom_h

#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/eeprom.h>

#include "basic.h"

void flash_read_page(uint8_t page_index, void* data)
{
	size_t page_address = page_index * FLASH_PAGE_SIZE;

	uint8_t* bytes = data;
	for (uint8_t byte_index = 0; byte_index < FLASH_PAGE_SIZE; byte_index++)
		*bytes++ = pgm_read_byte(page_address + byte_index);
}
void flash_write_page(uint8_t page_index, void* data)
{
	size_t page_address = page_index * FLASH_PAGE_SIZE;

	boot_page_erase(page_address);
	boot_spm_busy_wait();

	uint8_t* bytes = data;
	for (uint8_t byte_index = 0; byte_index < FLASH_PAGE_SIZE; byte_index += 2)
		boot_page_fill(byte_index, *bytes++ << 0 | *bytes++ << 8);

	boot_page_write(page_address);
	boot_spm_busy_wait();

	boot_rww_enable();
}

uint8_t decode_byte(void** offset)
{
	uint8_t value = eeprom_read_byte(*offset);

	*offset += 1;

	return value;
}
uint16_t decode_word(void** offset)
{
	uint16_t value = eeprom_read_word(*offset);

	*offset += 2;

	return value;
}
uint32_t decode_dword(void** offset)
{
	uint32_t value = eeprom_read_dword(*offset);

	*offset += 4;

	return value;
}

#endif

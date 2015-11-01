#ifndef eeprom_h
#define eeprom_h

#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/eeprom.h>

#include "basic.h"

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
	// TODO: what happens if this is used beyond the eeprom area? how does this affect verification ack?
	eeprom_read_block(data, EEPROM_BASE + page_index * EEPROM_PAGE_LENGTH, EEPROM_PAGE_LENGTH);
}
void eeprom_write_page(uint8_t page_index, void* data)
{
	eeprom_update_block(EEPROM_BASE + page_index * EEPROM_PAGE_LENGTH, data, EEPROM_PAGE_LENGTH);
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

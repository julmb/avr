#ifndef eeprom_h
#define eeprom_h

#ifndef __AVR_ATmega328P__
	#error "The eeprom module is not available on the chosen MCU."
#endif

#include <avr/eeprom.h>

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

#endif

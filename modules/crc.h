#ifndef crc_h
#define crc_h

#include <stddef.h>
#include <stdint.h>

#include <util/crc16.h>

uint16_t crc16_byte(uint8_t byte, uint16_t value)
{
	return _crc16_update(value, byte);
}
uint16_t crc16(void* data, size_t length, uint16_t value)
{
	uint8_t* bytes = data;

	for (size_t index = 0; index < length; index++) value = crc16_byte(*bytes++, value);

	return value;
}

#endif

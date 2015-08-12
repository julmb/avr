#ifndef usi_spi_h
#define usi_spi_h

#include <avr/io.h>

void spi_initialize()
{
	// configure USI for SPI-compatible three-wire mode
	// set wire mode to 01
	// set clock source to 10
	USICR = (0 << USIWM1) | (1 << USIWM0) | (1 << USICS1) | (0 << USICS0);
	
	// reset counter
	USISR &= ~_BV(USICNT3) & ~_BV(USICNT2) & ~_BV(USICNT1) & ~_BV(USICNT0);
}
void spi_dispose() { }

uint8_t spi_read_byte()
{
	// clear counter overflow flag by writing a one to it
	USISR |= _BV(USIOIF);

	// wait for the next counter overflow
	while (!(USISR & _BV(USIOIF)));

	// read the USI data register
	return USIDR;
}

void spi_read_bytes(uint8_t* data, uint16_t count)
{
	for (uint16_t index = 0; index < count; index++) data[index] = spi_read_byte();
}

#endif

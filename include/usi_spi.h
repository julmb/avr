#ifndef usi_spi_h
#define usi_spi_h

#ifndef __AVR_ATtiny85__
	#error "The USI SPI module is not available on the chosen MCU."
#endif

#include <avr/io.h>

uint8_t spi_read_byte()
{
	// clear counter overflow flag by writing a one to it
	USISR |= _BV(USIOIF);

	// wait for the next counter overflow
	while (!(USISR & _BV(USIOIF)));

	// read the USI data register
	return USIDR;
}
void spi_read_bytes(uint8_t* data, uint16_t length)
{
	for (uint16_t index = 0; index < length; index++) data[index] = spi_read_byte();
}

void spi_initialize()
{
	// configure USI for SPI-compatible three-wire mode
	// set wire mode to 01
	// set clock source to 10
	USICR = (0 << USIWM1) | (1 << USIWM0) | (1 << USICS1) | (0 << USICS0);
	
	// reset counter
	USISR &= ~_BV(USICNT3) & ~_BV(USICNT2) & ~_BV(USICNT1) & ~_BV(USICNT0);

	// reset USI data
	USIDR = 0x00;
}
void spi_dispose()
{
	// reset USI configuration
	USICR = 0x00;

	// reset USI status
	USISR = 0x00;

	// reset USI data
	USIDR = 0x00;
}

#endif

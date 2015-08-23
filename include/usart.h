#ifndef usart_h
#define usart_h

#ifndef __AVR_ATmega328P__
	#error "The USART module is not available on the chosen MCU."
#endif

#include "port.h"

uint8_t usart_rx_has_byte()
{
	return UCSR0A & _BV(RXC0);
}
uint8_t usart_rx_get_byte()
{
	return UDR0;
}

void usart_rx_read_byte(uint8_t* data)
{
	while (!usart_rx_has_byte()) ;

	*data = usart_rx_get_byte();
}
void usart_rx_read_bytes(uint8_t* data, uint16_t data_length)
{
	for (uint16_t index = 0; index < data_length; index++) usart_rx_read_byte(&data[index]);
}

void usart_initialize(uint16_t baud_rate, uint8_t double_speed)
{
	// set UBBR0 to 2 for 833333 baud at 20 MHz CPU clock in double speed mode
	UBRR0 = baud_rate;

	// enable double speed mode (U2X0 = 0b1)
	// disable multi-processor communication mode (MPCM0 = 0b0)
	// disable all interrupts (RXCIE0 = 0b0, TXCIE0 = 0b0, UDRIE0 = 0b0)
	// enable receiver and transmitter (RXEN0 = 0b1, TXEN0 = 0b1)
	// set mode to asynchronous USART (UMSEL0 = 0b00)
	// set parity mode to disabled (UPM0 = 0b00)
	// set stop bit mode to 1 stop bit (USBS0 = 0b0)
	// set character size to 8 bit (UCSZ0 = 0b011)
	// set clock polarity bit to zero, not used in asynchronous mode (UCPOL0 = 0b0)
	UCSR0A = ((double_speed ? 1 : 0) << U2X0) | (0 << MPCM0);
	UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0) | (0 << UCSZ02);
	UCSR0C = (0 << UMSEL01) | (0 << UMSEL00) | (0 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0);
}
void usart_dispose()
{
	// reset baud rate
	UBBR0 = 0x0000;

	// reset USART configuration and status
	UCSR0A = 0x00;
	UCSR0B = 0x00;
	UCSR0C = 0x00;

	// reset USART data
	UDR0 = 0x00;
}

#endif

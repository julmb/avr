#ifndef usart_h
#define usart_h

#ifndef __AVR_ATmega328P__
	#error "The USART module is not available on the chosen MCU."
#endif

uint8_t usart_rx_has_byte()
{
	return UCSR0A & _BV(RXC0);
}
uint8_t usart_rx_get_byte()
{
	return UDR0;
}

void usart_rx_read_bytes(void* data, size_t length)
{
	uint8_t* bytes = data;

	for (uint16_t index = 0; index < length; index++)
	{
		while (!usart_rx_has_byte()) ;

		*bytes++ = usart_rx_get_byte();
	}
}

void usart_initialize(uint8_t receiver, uint8_t transmitter, uint16_t divider, uint8_t double_speed)
{
	// set double speed mode (U2X0 = double_speed)
	// disable multi-processor communication mode (MPCM0 = 0b0)
	// disable all interrupts (RXCIE0 = 0b0, TXCIE0 = 0b0, UDRIE0 = 0b0)
	// set receiver and transmitter (RXEN0 = receiver, TXEN0 = transmitter)
	// set mode to asynchronous USART (UMSEL0 = 0b00)
	// set parity mode to disabled (UPM0 = 0b00)
	// set stop bit mode to 1 stop bit (USBS0 = 0b0)
	// set character size to 8 bit (UCSZ0 = 0b011)
	// set clock polarity bit to zero, not used in asynchronous mode (UCPOL0 = 0b0)
	UCSR0A = ((double_speed ? 1 : 0) << U2X0) | (0 << MPCM0);
	UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | ((receiver ? 1 : 0) << RXEN0) | ((transmitter ? 1 : 0) << TXEN0) | (0 << UCSZ02);
	UCSR0C = (0 << UMSEL01) | (0 << UMSEL00) | (0 << UPM01) | (0 << UPM00) | (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0);
	UBRR0 = divider;
}
void usart_dispose()
{
	// reset baud rate
	UBRR0 = 0x0000;

	// reset USART configuration and status
	UCSR0A = 0x00;
	UCSR0B = 0x00;
	UCSR0C = 0x00;

	// reset USART data
	UDR0 = 0x00;
}

#endif

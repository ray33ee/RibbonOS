#include <common/uart.h>

extern "C"
{
	
	
	void uart_init()
	{
	    mmio_write(UART0_CR, 0x00000000);

	    mmio_write(GPPUD, 0x00000000);
	    delay(150);

	    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	    delay(150);

	    mmio_write(GPPUDCLK0, 0x00000000);

	    mmio_write(UART0_ICR, 0x7FF);

#ifndef MODEL_1 //900Mhz clock speed, 115200 baud
	    mmio_write(UART0_IBRD, 325);
	    mmio_write(UART0_FBRD, 33);
#else //700Mhz clock speed, 115200 baud
	    mmio_write(UART0_IBRD, 379);
	    mmio_write(UART0_FBRD, 50);
#endif

	    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

	    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

	    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
	}

	void uart_putc(unsigned char c)
	{
	    while ( mmio_read(UART0_FR) & (1 << 5) ) { }
	    mmio_write(UART0_DR, c);
	}

	unsigned char uart_getc()
	{
	    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
	    return mmio_read(UART0_DR);
	}

	void uart_puts(const char* str)
	{
	    for (size_t i = 0; str[i] != '\0'; i ++)
	        uart_putc((unsigned char)str[i]);
	}

	/* Sends a hex string representing the number 'value' to the terminal */
	void uart_puti(uint32_t value)
	{
		auto numToHex = [](uint32_t num)-> char { return num < 10 ? '0' + num : 'A' + num - 10; };
		uint8_t* byte = (uint8_t*)&value + 3;
		uart_putc('0');
		uart_putc('x');
		for (uint8_t i = 0; i < 4; ++i)
		{
			uart_putc(numToHex((*byte & 0xF0) >> 4));
			uart_putc(numToHex(*byte & 0x0F));
			--byte;
		}
	}

	void uart_putn()
	{
		uart_putc('\r');
		uart_putc('\n');

	}
}
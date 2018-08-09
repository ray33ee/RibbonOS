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

	    mmio_write(UART0_IBRD, 325);
	    mmio_write(UART0_FBRD, 33);

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
}
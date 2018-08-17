#include <common/uart.h>

#include <common/string.h>

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

unsigned char uart_getc()
{
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }
    return mmio_read(UART0_DR);
}


void uart_put(unsigned char c)
{
		while ( mmio_read(UART0_FR) & (1 << 5) ) { }
		mmio_write(UART0_DR, c);
}

/* Sends a hex string representing the number 'value' to the terminal */
void uart_put(uint32_t value)
{
	auto numToHex = [](uint32_t num)-> char { return num < 10 ? '0' + num : 'A' + num - 10; };
	uint8_t* byte = (uint8_t*)&value + 3;
	uart_put('0');
	uart_put('x');
	for (uint8_t i = 0; i < 4; ++i)
	{
		uart_put(numToHex((*byte & 0xF0) >> 4));
		uart_put(numToHex(*byte & 0x0F));
		--byte;
	}
}

void uart_put(int value)
{
	uart_put((uint32_t)value);
}

void uart_put()
{
	uart_put('\r');
	uart_put('\n');

}

void uart_put(char* str)
{
		for (size_t i = 0; str[i] != '\0'; i ++)
				uart_put((unsigned char)str[i]);
}

void uart_put(const char* str)
{
		for (size_t i = 0; str[i] != '\0'; i ++)
				uart_put((unsigned char)str[i]);
}

void uart_put(String str)
{
	uart_put(str.data());
}

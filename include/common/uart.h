#ifndef COMMON_UART
#define COMMON_UART

#include <cstddef>
#include <cstdint>
#include <common/mmio.h>
#include <cfloat>

float getDivider();

int32_t getIBRD();

int32_t getFBRD();

extern "C" {
	void uart_init();

	void uart_putc(unsigned char c);

	unsigned char uart_getc();

	void uart_puts(const char* str);

	void uart_puti(uint32_t value);

	void uart_putn();
}

#endif

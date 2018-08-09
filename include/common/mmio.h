#ifndef COMMON_MMIO
#define COMMON_MMIO

#include <stdint.h>

extern "C"
{
	void mmio_write(uint32_t reg, uint32_t data);

	uint32_t mmio_read(uint32_t reg);

	// Loop <delay> times in a way that the compiler won't optimize away
	void delay(int32_t count);
}

#endif
#ifndef KERNEL_KERNEL
#define KERNEL_KERNEL

#include <stdint.h>
#include <common/uart.h>
#include <common/mmio.h>

class Number;

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags);

#endif
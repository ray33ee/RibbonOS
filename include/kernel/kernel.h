#ifndef KERNEL_KERNEL
#define KERNEL_KERNEL

#include <stddef.h>
#include <stdint.h>
#include <common/uart.h>
#include <common/mmio.h>
#include <common/new.h>
#include <common/smart.h>

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags);

#endif
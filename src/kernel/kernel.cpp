#include <kernel/kernel.h>

extern "C" uint32_t addw(uint32_t a, uint32_t b);
extern "C" uint32_t subw(uint32_t a, uint32_t b);

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;
    heapInit();
    uart_init();

    uart_puts("Hello, kernel World!\r\n");

    SharedSmart<int> first(12);
    SharedSmart<int> second(99);

    second = first;

    while (1) uart_putc(uart_getc());
}

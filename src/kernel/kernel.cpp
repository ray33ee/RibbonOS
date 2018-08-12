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

    heapShow();

    int* arr = (int*)malloc(sizeof(int) * 1024 * 3);

    uart_puts("POINTER: ");
    uart_puti((uint32_t)arr);
    uart_putn();

    heapShow();

    arr = (int*)realloc(arr, 4096 * 1);

    uart_puts("POINTER: ");
    uart_puti((uint32_t)arr);
    uart_putn();

    heapShow();

    while (1) uart_putc(uart_getc());
}

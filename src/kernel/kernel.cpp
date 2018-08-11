#include <stddef.h>
#include <stdint.h>
#include <kernel/kernel.h>
#include <kernel/meminit.h>
#include <common/uart.h>
#include <common/new.h>

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;

    HeapMetadata heap;

    uart_init();
    uart_puts("Hello, kernel World!\r\n");

    heap.show();

    uart_putn();

    void* ptr = heap.malloc(4096 * 2);

    heap.show();

    heap.free(ptr, 4096 * 2);

    heap.show();

    while (1) {
        uart_putc(uart_getc());
        uart_putn();
    }
}

#include <stddef.h>
#include <stdint.h>
#include <kernel/kernel.h>
#include <common/uart.h>
#include <common/new.h>

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;
    heapInit();
    uart_init();



    uart_puts("Hello, kernel World!\r\n");

    heapShow();

    uart_putn();

    char* ptr1 = new char[1000];
    char* ptr2 = new char[4097];
    char* ptr3 = new char[1000];
    char* ptr4 = new char[1000];

    heapShow();

    delete[] ptr3;

    heapShow();

    char* ptr5 = new char[4097];

    heapShow();

    char* ptr6 = new char[1000];

    heapShow();

    int* n = new int;

    delete n;


    while (1) {
        uart_putc(uart_getc());
        uart_putn();
    }
}

#include <stddef.h>
#include <stdint.h>
#include <kernel/kernel.h>
#include <common/uart.h>

class Number
{
private:
    unsigned char ch;

public:
    Number(unsigned char c = 0): ch(c) {}

    void send() const { uart_putc(ch); }

};

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    uart_puts("Hello, kernel World!\r\n");

    Number f('#');

    f.send();

    while (1) {
        uart_putc(uart_getc());
        uart_putc('\n');
    }
}

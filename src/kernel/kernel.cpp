#include <kernel/kernel.h>
#include <common/smart.h>
#include <common/vector.h>
#include <common/linked.h>

extern "C" uint32_t addw(uint32_t a, uint32_t b);
extern "C" uint32_t subw(uint32_t a, uint32_t b);

class BigBoi
{
public:
    uint32_t arr[512];

    BigBoi() {}
};

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;
    heapInit();
    uart_init();

    uart_puts("Hello, kernel World!\r\n");

    Linked<int> list;

    list.append(23);

    list.append(17);

    list.append(33);

    for (auto it = list.const_begin(); it != list.const_end(); ++it)
    {
        uart_puti((uint32_t)*it);
        uart_putn();
    }

    while (1) uart_putc(uart_getc());
}

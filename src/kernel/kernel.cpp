#include <kernel/kernel.h>
#include <common/smart.h>
#include <common/vector.h>
#include <common/linked.h>
#include <common/string.h>

extern "C" uint32_t addw(uint32_t a, uint32_t b);
extern "C" uint32_t subw(uint32_t a, uint32_t b);

struct Test
{
  int number;
  Test(int n = 0): number(n) {}
  void show() const { uart_put(number); uart_put(); }
};

extern "C" void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;
    heapInit();
    uart_init();

    uart_put("Hello, kernel World!\r\n");

    String str("hello world!");

		uart_put(str);

		uart_put('d');

    while (1) uart_put(uart_getc());
}

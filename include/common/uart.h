#ifndef COMMON_UART
#define COMMON_UART

#include <cstddef>
#include <cstdint>
#include <common/mmio.h>
#include <cfloat>

class String;

float getDivider();

int32_t getIBRD();

int32_t getFBRD();

void uart_init();

unsigned char uart_getc();

void uart_put(unsigned char c);

void uart_put(uint32_t value);

void uart_put(int value);

void uart_put();

void uart_put(char* str);

void uart_put(const char* str);

void uart_put(String str);

#endif

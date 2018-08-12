#ifndef COMMON_NEW
#define COMMON_NEW

#include <cstddef>
#include <kernel/meminit.h>

#include <common/uart.h>

void* operator new (size_t size);

void* operator new[] (size_t size);

void operator delete (void* ptr, unsigned int);

void operator delete[] (void* ptr);

#endif
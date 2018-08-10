#ifndef COMMON_NEW
#define COMMON_NEW

#include <cstddef>
#include <common/uart.h>
#include <kernel/meminit.h>

extern "C"
{
	void* kmalloc(size_t size);

	void kfree(void* ptr, size_t size);

	void* krealloc(size_t size);

	void* kcalloc(size_t size);
}

void* operator new (size_t size);

void* operator new[] (size_t size);

void operator delete (void* ptr, size_t size);

void operator delete[] (void* ptr, size_t size);

#endif
#ifndef KERNEL_MEMINIT
#define KERNEL_MEMINIT

#include <cstdint>
#include <cstddef>

#ifdef DEBUG
#include <common/uart.h>
#endif

static const uint32_t KERNEL_RESERVED = 1024 * 1024 * 2;
static const uint32_t PAGE_SIZE = 4096;
static const uint32_t LEN = 10;

struct PageData
{
	/* Page metadata */
	uint8_t _allocated : 1;
	uint8_t _kernel : 1;
	uint32_t _reserved : 30;

	/* Addressed to next blocks in list */
	uint32_t _next;
	uint32_t _previous;
};




extern PageData* _meta; //Pointer to beginning of metadata list
extern uint32_t _base; //Pointer to beginning of heap

void memclr(void*, size_t);
void memcpy(void*, void*, size_t);

void heapInit();

uint32_t getIndex(void* ptr);
uint32_t getPagesRequired(size_t);

#ifdef DEBUG
void heapShow();
#endif

void* malloc(size_t);
void free(void*);

void* calloc(size_t);
void* realloc(void*, size_t);


#endif

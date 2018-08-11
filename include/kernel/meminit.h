#ifndef KERNEL_MEMINIT
#define KERNEL_MEMINIT

#include <cstdint>

//Debugging ONLY
#include <common/uart.h>

class HeapMetadata;

class PageData
{
	/* Allow access to PageData through HeapMetadata */
	friend HeapMetadata;
private:
	/* Page metadata */
	uint8_t _allocated : 1;	
	uint8_t _kernel : 1;		
	uint32_t _reserved : 30;	

	/* Addressed to next blocks in list */
	uint32_t _next;				
	uint32_t _previous;			
public: 
	/* Setup linked-list pointers and metadata */
	PageData();

	bool isAllocated() const;
	bool isKernel() const;

	uint32_t next() const;
	uint32_t previous() const;
};

class HeapMetadata
{
private:
	static const uint32_t LEN = 20;
	static const uint32_t PAGE_SIZE = 4096;
	PageData _meta[LEN+1];						//Add one for past the end pointer
	uint32_t _base; //Address of bottom of heap

public:
	HeapMetadata();

	void show() const;

	void* malloc(size_t);
	void free(void*, size_t);
};

#endif
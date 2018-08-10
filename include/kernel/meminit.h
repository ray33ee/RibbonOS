#ifndef KERNEL_MEMINIT
#define KERNEL_MEMINIT

#include <cstdint>

//Debugging ONLY
#include <common/uart.h>

class HeapMetadata;

class PageData
{
	friend HeapMetadata;
private:
	uint8_t _allocated : 1;	//Set if block is allocated
	uint8_t _kernel : 1;		//Set if owned by kernel
	uint32_t _reserved : 30;	
	uint32_t _next;				//Next block in list
	uint32_t _previous;			//Previous block in list
public:
	PageData();

	bool isAllocated() const;
	bool isKernel() const;

	uint32_t next() const;
	uint32_t previous() const;
};

class HeapMetadata
{
private:
	static const uint32_t LEN = 15;
	PageData _meta[LEN];
public:
	HeapMetadata();
};

extern "C" 
{
	uint32_t getMemSize();

	uint32_t getHeapSize();

	void initialise_memory();
}

#endif
#include <kernel/meminit.h>

PageData::PageData()
{
	//Cast the position of this to integer, then subtract or add sizeof(this) many bytes to point to position of next or previous object
	_previous = (uint32_t)this - sizeof(*this);
	_next = (uint32_t)this + sizeof(*this);
}

bool PageData::isAllocated() const
{
	return _allocated;
}

bool PageData::isKernel() const
{
	return _kernel;
}

uint32_t PageData::next() const
{
	return _next;
}

uint32_t PageData::previous() const
{
	return _previous;
}

HeapMetadata::HeapMetadata()
{
	_meta[0]._previous = 0;
	_meta[LEN-1]._next = 0;

	PageData* start = _meta;

	uart_puti((uint32_t)start);
	uart_putc('\r');
	uart_putc('\n');

	for (;start->_next != 0;)
	{

		uart_puti(start->_next);
		uart_putc('\r');
		uart_putc('\n');
		start = (PageData*)start->_next;

	}
}

extern "C" 
{
	const uint32_t PAGE_SIZE = 4096; //Page size of memory

	uint32_t __end;

	uint32_t getMemSize()
	{
		return 1024*1024*256;
	}

	uint32_t getHeapSize()
	{
		return __end - getMemSize();
	}

	void initialise_memory()
	{
		uint32_t total = getHeapSize();
		uint32_t page_count = total / PAGE_SIZE;
	}
}
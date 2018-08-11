#include <kernel/meminit.h>

extern "C" uint32_t __end;

PageData::PageData()
{
	//Increment to next node in list, then cast back to uint32_t
	_previous = uint32_t(this - 1);
	_next = uint32_t(this + 1);

	//Clear metadata
	_allocated = 0;
	_kernel = 0;
	_reserved = 0;
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
	//Set head and tail pointers to null
	_meta[0]._previous = 0;
	_meta[LEN]._next = 0;

	//Set _base to the bottom of the heap, the area of the code after the program, __end
	_base = __end;

	_meta[1]._allocated = 1;
}

void HeapMetadata::show() const
{
	for (const PageData* start = _meta; start->_next != 0; start = (PageData*)start->_next)
	{
		uart_puti((uint32_t)start);
		uart_puts(start->_allocated == 0 ? " FREE" : " ALLOC");
		uart_putn();
	}
}

void* HeapMetadata::malloc(size_t size)
{
	if (!size)
		return nullptr;

	//Calculate number of pages required 
	size_t pages = size / PAGE_SIZE + bool(size % PAGE_SIZE); //Equivalent of ceil(size / PAGE_SIZE)

	//Locate available page (making sure the required number of pages after are unallocated)
	PageData* candidate = &_meta[LEN];
	for (PageData* iter = _meta; iter->_next != 0; iter = (PageData*)iter->_next) //Iterate over all blocks
	{
		if (!iter->_allocated) //Make sure the required number of pages are free
		{
			bool unallocated = true;
			candidate = iter;
			for (uint32_t i = 0; i < pages; ++i)
			{
				unallocated &= !iter->_allocated;
				if (iter->_allocated) //If a block is allocated, break and continue search
					break;
				iter = (PageData*)iter->_next;
			}
			if (unallocated) //If ALL required pages are free, stop the search
				break;
		}
	}

	//Get first page and page after block
	auto first = candidate;
	auto last = candidate + pages;

	//Get nth index of metadata index
	auto index = (PageData*)first - _meta; 

	//If the allocation is past the end, there is no room for this allocation
	if (index + pages > LEN) 
	{
		uart_puts("MALLOC FAILED\r\n");
		return nullptr;
	}

	//Identify block as allocated
	candidate->_allocated = 1;

	//Link up outer nodex
	first->_next = (uint32_t)last;
	last->_previous = (uint32_t)first;

	//return address to beginning of allocated block
	return (void*)(_base + PAGE_SIZE * index);
}

void HeapMetadata::free(void* ptr, size_t size)
{
	if (ptr == nullptr)
		return;

	//1. Get position of PageData node from ptr
	auto index = ((uint32_t)ptr - _base) / PAGE_SIZE; //Get nth page index
	size_t pages = size / PAGE_SIZE + bool(size % PAGE_SIZE); //Equivalent of ceil(size / PAGE_SIZE)

	auto first = _meta + index;
	auto last = first + pages;

	uart_puti((uint32_t)first);
	uart_putn();

	//2. Re-link nodes
	first->_next = uint32_t(first+1);
	first->_previous = uint32_t(first-1);


	//3. Set metadata
	first->_allocated = 0;
}
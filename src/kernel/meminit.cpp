#include <kernel/meminit.h>

extern "C" uint32_t __end;

PageData* _meta;
uint32_t _base;

extern "C" 
{
	void heapInit()
	{
		//Set _metadata array to area of meemory directly after program, at __end
		_meta = (PageData*)__end;

		//Set user heap to after kernel reserved space, 2MB after __end
		_base = (uint32_t)__end + KERNEL_RESERVED;

		//Setup all metadata enteries
		for (int i = 0; i < LEN; ++i)
		{
			//Increment to next node in list, then cast back to uint32_t
			_meta[i]._previous = (uint32_t(_meta + i - 1));
			_meta[i]._next = (uint32_t(_meta + i + 1));

			//Clear metadata
			_meta[i]._allocated = (0);
			_meta[i]._kernel = (0);

			_meta[i]._reserved = 0;
		}

		//Set head and tail pointers to null
		_meta[0]._previous = 0;
		_meta[LEN]._next = 0;
	}

#ifdef DEBUG
	void heapShow()
	{
		for (const PageData* start = _meta; start->_next != 0; start = (PageData*)start->_next)
		{
			uart_puti((uint32_t)start);
			uart_puts(start->_allocated == 0 ? " FREE" : " ALLOC");
			uart_putn();
		}
		uart_putn();
	}
#endif

	void* malloc(size_t size)
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
#ifdef DEBUG
			uart_puts("MALLOC FAILED\r\n");
#endif
			return nullptr;
		}

		//Identify block as allocated
		candidate->_allocated = (1);

		//Link up outer nodex
		first->_next = ((uint32_t)last);
		last->_previous = ((uint32_t)first);

		//return address to beginning of allocated block
		return (void*)(_base + PAGE_SIZE * index);
	}

	void free(void* ptr)
	{
		//0. Make sure ptr is valid
		//NOTE: As per C/C++ standards, no bounds checking is performed
		if (ptr == nullptr)
			return;

		//1. Get position of PageData node from ptr
		auto index = ((uint32_t)ptr - _base) / PAGE_SIZE; //Get nth page index

		//2. Get first and last metadata enteries
		auto first = _meta + index;
		auto last = (PageData*)first->_next;

		//3. Re-link nodes
		first->_next = (uint32_t(first+1));
		last->_previous = (uint32_t(first-1));

		//4. Set metadata
		first->_allocated = (0);
	}
}
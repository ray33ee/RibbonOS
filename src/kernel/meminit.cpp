#include <kernel/meminit.h>

extern "C" uint32_t __end;

PageData* _meta;
uint32_t _base;

extern "C" 
{
	void memclr(void* start, size_t len)
	{
		char* arr = (char*)start;
		for (size_t i = 0; i < len; ++i)
			arr[i] = 0;
	}

	void memcpy(void* source, void* estination, size_t len)
	{

	}

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
			_meta[i]._allocated = false;
			_meta[i]._kernel = false;

			_meta[i]._reserved = 0;
		}

		//Set head and tail pointers to null
		_meta[0]._previous = 0;
		_meta[LEN]._next = 0;
	}

	uint32_t getIndex(void* ptr)
	{
		return ((uint32_t)ptr - _base) / PAGE_SIZE;
	}

	//Calculates the number of pages required to fit 'size' bytes of data
	uint32_t getPagesRequired(size_t size)
	{
		return size / PAGE_SIZE + bool(size % PAGE_SIZE);
	}

#ifdef DEBUG
	void heapShow()
	{
		for (const PageData* start = _meta; start->_next != 0; start = (PageData*)start->_next)
		{
			uart_puti((uint32_t)start);
			uart_puts(start->_allocated ? " ALLOC" : " FREE");
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
		uint32_t pages = getPagesRequired(size); //Equivalent of ceil(size / PAGE_SIZE)

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
		candidate->_allocated = true;

		//Link up outer nodex
		first->_next = ((uint32_t)last);
		last->_previous = ((uint32_t)first);

		//return address to beginning of allocated block
		return (void*)(_base + PAGE_SIZE * index);
	}

	void free(void* ptr)
	{
		//0. Make sure ptr is valid
		//NOTE: As per C/C++ standards, no bounds checking is performed, just a nullptr check
		if (ptr == nullptr)
			return;

		//1. Get position of PageData node from ptr
		auto index = getIndex(ptr); //Get nth page index

		//2. Get first and last metadata enteries
		auto first = _meta + index;
		auto last = (PageData*)first->_next;

		//3. Re-link nodes
		first->_next = (uint32_t(first+1));
		last->_previous = (uint32_t(first-1));

		//4. Set metadata
		first->_allocated = false;
	}

	void* calloc(size_t size)
	{
		void* ptr = malloc(size);

		memclr(ptr, size);

		return ptr;
	}

	void* realloc(void* ptr, size_t newsize)
	{
		if (ptr == nullptr)
			return malloc(newsize);

		if (!newsize)
		{
			free(ptr);
			return nullptr;
		}

		//1. Calculate the number of pages required by the new size
		auto pagesRequired = getPagesRequired(newsize);

		//2. Calculate the index of the pointer
		auto index = getIndex(ptr);

		//3. Calculate the number of pages currently taken by ptr
		PageData* start = _meta + index;
		PageData* end = (PageData*)start->_next;

		auto currentPages = (end - start);

		//Private function used to allocate new block
		auto lambdaMalloc = [=]()-> void*
		{
			//i) Allocate new block
			uint32_t* newptr = (uint32_t*)malloc(newsize);

			//ii) Copy data from old block to new
			uint32_t* recast = (uint32_t*)ptr; //Cast to uint32 to copy data
			for (int i = 0; i < currentPages*PAGE_SIZE / sizeof(uint32_t); ++i)
				newptr[i] = recast[i];

			//iii) Free old block
			free(ptr);

			//iv) Return new pointer
			return newptr;
		};

		//4. If we don't have enough pages:
		if (pagesRequired > currentPages)
		{
			//a) Probe subsequent blocks for space to expand
			bool isFree = true;
			auto extra = pagesRequired - currentPages;

			//No room for extension. Must reallocate
			if (index + extra > LEN) 
				return lambdaMalloc();

			for (int i = 0; i < extra; ++i)
				isFree &= !end[i]._allocated;
			
			//b) If there is room to expand, update nodes accordingly, return original ptr
			if (isFree)
			{
				PageData* newEnd = start + pagesRequired; //New 'next' pointer after extension.

				start->_next = (uint32_t)newEnd;
				newEnd->_previous = (uint32_t)start;
			}
			//c) If there is no room to expand:
			else
			{
				return lambdaMalloc();
			}
		}
		//6. If we have too many pages
		else if (pagesRequired < currentPages)
		{
			//a) Shortened block by updating nodes. return ptr
			end->_previous = (uint32_t)(end-1);
			start->_next = (uint32_t)(start + pagesRequired);
		}
		return ptr;
	}

	
}
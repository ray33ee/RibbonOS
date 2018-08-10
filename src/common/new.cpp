#include <common/new.h>


extern "C"
{

	void* kmalloc(size_t size)
	{
		return 0;
	}

	void kfree(void* ptr, size_t size)
	{

	}

	void* krealloc(size_t size)
	{
		return 0;
	}

	void* kcalloc(size_t size)
	{
		return 0;
	}
}

void* operator new (size_t size)
{
	return kmalloc(size);
}

void* operator new[] (size_t size)
{
	return kmalloc(size);
}

void operator delete (void* ptr, size_t size)
{
	kfree(ptr, size);
}

void operator delete[] (void* ptr, size_t size)
{
	kfree(ptr, size);
}
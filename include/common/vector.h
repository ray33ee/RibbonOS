#ifndef COMMON_VECTOR
#define COMMON_VECTOR

#include <kernel/meminit.h> //for getPagesRequired and realloc

template<typename T>
class Vector
{
protected:
	T* 	_array;
	int _count;
	int _increase; //How many times the vector has had to increase size

public:

	class iterator
	{
	protected:
		T* _element;
	public:
		/* Constructors */
		iterator(): _element(nullptr) {}

		iterator(T* element): _element(element) {}

		/* Copying */
		iterator(const iterator &iter): _element(iter._element) {}

		iterator& operator=(const iterator &iter)	{ _element = iter._element;	return this; }

		/* Comparison */
		bool operator==(const iterator &iter) { return _element == iter._element; }
		bool operator==(T* ptr) { return _element == ptr; }

		bool operator!=(const iterator &iter) { return _element != iter._element; }
		bool operator!=(T* ptr) { return _element != ptr; }

		/* Increment */
		iterator operator++(int) { auto old = *this; ++_element; return old; }
		iterator& operator++() { ++_element; return *this; }

		/* Decrement */
		iterator operator--(int) { auto old = *this; --_element; return old; }
		iterator& operator--() { --_element; return *this; }

		/* Pointer stuff */
		T& operator*() { return *_element; }
		T* operator->() { return _element; }

		/* Conversion */
		operator bool() const { return _element == nullptr; }

		operator void*() const { return (void*)_element; }

	};

	class const_iterator : public iterator
	{
	public:
		const_iterator() : iterator() {}

		const_iterator(T* element) : iterator(element) {}

		const_iterator(const const_iterator &iter): iterator(iter._element) {}

		const T& operator*() { return *iterator::_element; }
		const T* operator->() { return iterator::_element; }

		operator const void*() const { return (void*)iterator::_element; }
	};

	/* Constructors */
	Vector() { _array = new T[1]; _count = 0; _increase = 0; }

	Vector(T* array, int count) { _array = array; _count = count; _increase = 0; }

	/* Copy Constructor */
	Vector(const Vector &list) { _array = list._array; _count = list._count; _increase = list._increase; }

	/* Destructor */
	~Vector() { delete[] _array; }

	/* Get accessor methods */
	int length() const { return _count; }

	T* data() const { return _array; }

	T at(int ind) const { return _array[ind]; }

	T& operator[](int ind) { return _array + ind; }
	const T& operator[](int ind) const { return _array + ind; }

	/* Array manipulation */
	void append(const T &obj)
	{
		increase(1);
		//4. Add the item on to the end of the list
		_array[_count] = obj;
		//5. Increment _count
		++_count;
	}

	void remove(int index)
	{
		for (int i = index; i < _count; ++i)
			_array[i] = _array[i+1];
		--_count;
		squeeze();
	}

	void insert(int index, const T &val)
	{
		if (index == _count)
		{
			append(val);
		}
		else
		{
			increase(1);
			for (int i = _count; i >= index; ++i)
				_array[i] = _array[i-1];
			_array[index] = val;
			++_count;
		}
	}

	/* Memory Managment */
	void reserve(size_t size) { _array = realloc(_array, size * sizeof(T)); }

	void squeeze();

	void increase(size_t size) //Request an expansion of 'size' bytes
	{
		//1. Calculate current size in bytes
		auto bytesize = _count * sizeof(T);
		//2. Calculate current and future size in pages
		auto currentPageSize = getPagesRequired(bytesize);
		auto futurePageSize = getPagesRequired(bytesize + sizeof(T) * size);
		//3. If the current append will spill over the allocation,
		if (currentPageSize < futurePageSize)
		{
			//a) Reallocate with 'current size in pages + 2^_increase' number of pages
			_array = (T*)realloc(_array, bytesize + getPagesRequired(sizeof(T) * size) * PAGE_SIZE * (1 << _increase));
			//b) Increment _increase
			++_increase;
		}
	}

	/* Iterators */
	iterator begin() const { return iterator(_array); }
	iterator end() const { return iterator(_array + _count); }

	const_iterator const_begin() const { return const_iterator(_array); }
	const_iterator const_end() const { return const_iterator(_array + _count); }

	/* Assignment */
	Vector& operator= (const Vector &list) { _array = list._array; _count = list._count; _increase = list._increase; }

	/* Deep copy of *this */
	Vector deep()
	{
		Vector<T> copy;

		copy._count = _count;
		copy._increase = _increase;

		copy.reserve(_count);

		for (int i = 0; i < _count; ++i)
			copy._array[i] = _array[i];

		return copy;
	}

};

#endif

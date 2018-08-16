#ifndef COMMON_SMART
#define COMMON_SMART

#include <common/uart.h>
#include <cstddef> //For nullptr_t

template <typename T>
struct RefObj
{
	int _refcount;
	T 	_object;
};

template <typename T>
class SharedSmart
{
private:
	/* Object containing reference count and object (type T) to point to */
	RefObj<T>* _pointee;

	/* If an object has been destroyed or copied, use this function to decrement reference count, and delete if needed */
	void decref() const
	{
		if (_pointee->_refcount == 1) 	//If this is the last object referencing _pointee, delete
		{
			delete _pointee;
			_pointee = nullptr;
		}
		else							//Otherwise just decrement ref count
			--(_pointee->_refcount);
	}

	/* If copied (via copy constructor or assignment) replace current pointer with pointer from 'smart' */
	void replace(SharedSmart<T> &smart)
	{
		//Decrement reference count, delete if needed
		decref();

		//Update pointee to new pointee
		_pointee = smart._pointee;

		//Increment new reference count
		++(_pointee->_refcount);
	}

public:

	/* Constructors */
	SharedSmart()
	{
		_pointee = new RefObj<T>;
		_pointee->_refcount = 1;
	}

	SharedSmart(T val) : SharedSmart() { _pointee->_object = val; }

	SharedSmart(SharedSmart<T>& smart) { replace(smart); }

	/* Prevent potentially dangerous raw pointer usage */
	SharedSmart(T* ptr) = delete;

	/* Get accessor methods */
	T* get() const { return &(_pointee->_object); }

	/* Destructor */
	~SharedSmart() { decref(); }

	/* Assignment */
	SharedSmart<T>& operator= (SharedSmart<T>& smart) { replace(smart); }

	/* Dereference pointer */
	T& operator* () const { return _pointee->_object; }

	/* Member access */
	T* operator-> () const { return &(_pointee->_object); }

	/* Comparison */
	bool operator==(const SharedSmart &smart) { return _pointee == smart._pointee; }

	bool operator==(nullptr_t null) { return _pointee == null; }

	bool operator!=(const SharedSmart &smart) { return _pointee != smart._pointee; }

	bool operator!=(nullptr_t null) { return _pointee != null; }

	/* Conversion */
	operator bool() const { return _pointee == nullptr; }

#ifdef DEBUG
	int getRefCount() const { return _pointee->_refcount; }

	RefObj<T>* getPointer() const { return _pointee; }
#endif

};

#endif

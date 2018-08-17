#ifndef COMMON_STRING
#define  COMMON_STRING

#include <common/vector.h>

class String : public Vector<char>
{
public:
	String(): Vector() { _array[0] = '\0'; }
	String(const char* str)
	{
		//Get number of characters in list
	  _count = 0;
		for (; str[_count] != '\0'; ++_count);

		//Allocate space for string
		_array = new char[_count+1];

		//Copy str to array
		for (int i = 0; i < _count; ++i)
			_array[i] = str[i];

		//Insert null character
		_array[_count] = '\0';

		//Increment count to include null character
		++_count;
	}
};

#endif

#ifndef COMMON_LINKED
#define COMMON_LINKED

template<typename T>
class Linked;

template<typename T>
class Node
{
private:

	friend class Linked<T>;

	Node<T>* _next;
	Node<T>* _prev;
	T 	  _data;
public:
	Node<T>* next() const { return _next; }
	Node<T>* prev() const { return _prev; }

	const T& data() const { return _data; }
};

/* Doubly linked list */
template <typename T>
class Linked
{
private:
	Node<T>* _top;
	Node<T>* _bottom;
public:

	class iterator
	{
	protected:
		Node<T>* _node;
	public:
		/* Constructors */
		iterator(): _node(nullptr) {}

		iterator(Node<T>* node): _node(node) {}

		/* Copying */
		iterator(const iterator &iter): _node(iter._node) {}

		iterator& operator=(const iterator &iter)	{ _node = iter._node;	return this; }

		/* Comparison */
		bool operator==(const iterator &iter) { return _node == iter._node; }
		bool operator==(T* ptr) { return &(_node->_data) == ptr; }

		bool operator!=(const iterator &iter) { return _node != iter._node; }
		bool operator!=(T* ptr) { return &(_node->_data) != ptr; }

		/* Increment */
		iterator operator++(int) { auto old = *this; _node = _node->_next; return old; }
		iterator& operator++() { _node = _node->_next; return *this; }

		/* Decrement */
		iterator operator--(int) { auto old = *this; _node = _node->_prev; return old; }
		iterator& operator--() { _node = _node->_prev; return *this; }

		/* Pointer stuff */
		T& operator*() { return _node->_data; }
		T* operator->() { return &_node->_data; }

		/* Conversion */
		operator bool() const { return _node == nullptr; }

		operator void*() const { return (void*)&(_node->_data); }

	};

	class const_iterator : public iterator
	{
	public:
		const_iterator() : iterator() {}

		const_iterator(Node<T>* node) : iterator(node) {}

		const_iterator(const const_iterator &iter): iterator(iter._node) {}

		const T& operator*() { return iterator::_node->_data; }
		const T* operator->() { return &iterator::_node->_data; }

		operator const void*() const { return (void*)iterator::_element; }
	};

	Linked()
	{
		_top = new Node<T>;
		_bottom = new Node<T>;

		_top->_next = nullptr;
		_top->_prev = _bottom;

		_bottom->_next = _top;
		_bottom->_prev = nullptr;
	}

	void append(const T &val)
	{
		//Create new node
		Node<T>* newnode = new Node<T>;

		//Get pointer to last item in list
		Node<T>* last = _top->_prev;

		//Link up new node to last node and top node
		newnode->_prev = last;
		newnode->_next = _top;

		//Link last node and head node to new node
		last->_next = newnode;
		_top->_prev = newnode;

		//Assign value
		newnode->_data = val;
	}

	void remove()
	{

	}

	int count() const {  }

	/* Iterators */
	iterator begin() const { return iterator(_bottom->_next); }
	iterator end() const { return iterator(_top); }

	const_iterator const_begin() const { return const_iterator(_bottom->_next); }
	const_iterator const_end() const { return const_iterator(_top); }

	~Linked();

};

#endif

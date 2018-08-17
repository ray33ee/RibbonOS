#ifndef COMMON_LINKED
#define COMMON_LINKED

/* Doubly linked list */
template <typename T>
class Linked
{
private:
	class Node
	{
	private:
		friend class iterator;
		friend class const_iterator;
		friend class Linked;

		Node* _next;
		Node* _prev;
		T 	  _data;
	public:
		Node() {}
	};

	Node* _top;
	Node* _bottom;

	Node* getNode(int ind) const
	{
		Node* node = _bottom;
		for (; ind >= 0; --ind)
			node = node->_next;
		return node;
	}
public:

	class iterator
	{
	protected:
		Node* _node;
	public:
		/* Constructors */
		iterator(): _node(nullptr) {}

		iterator(Node* node): _node(node) {}

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

		const_iterator(Node* node) { iterator::_node = node; }

		const_iterator(const const_iterator &iter): iterator(iter._node) {}

		const T& operator*() { return iterator::_node->_data; }
		const T* operator->() { return &iterator::_node->_data; }

		operator const void*() const { return (void*)iterator::_element; }
	};

	/* Constructor */
	Linked()
	{
		_top = new Node;
		_bottom = new Node;

		_top->_next = nullptr;
		_top->_prev = _bottom;

		_bottom->_next = _top;
		_bottom->_prev = nullptr;
	}

	/* Container manipulation */
	void append(const T &val)
	{
		//Create new node
		Node* newnode = new Node;

		//Get pointer to last item in list
		Node* last = _top->_prev;

		//Link up new node to last node and top node
		newnode->_prev = last;
		newnode->_next = _top;

		//Link last node and head node to new node
		last->_next = newnode;
		_top->_prev = newnode;

		//Assign value
		newnode->_data = val;
	}

	void remove(int index) const
	{
		auto node = getNode(index);

		node->_prev->_next = node->_next;
		node->_next->_prev = node->_prev;

		delete node;
	}

	void insert(int index, const T &val) const
	{
		//Get required node
		auto node = getNode(index);

		//Create new node
		Node* newnode = new Node;

		//Setup links from outer nodes to newnode
		node->_prev->_next = newnode;
		node->_next->_prev = newnode;

		//Setup links from newnode to outer nodes
		newnode->_prev = node->_prev;
		newnode->_next = node->_next;

		delete node;
	}

	/* Get accessor methods */
	int length() const
	{
		int count = 0;
		for (const_iterator it = const_begin(); it != const_end(); ++it)
			++count;
		return count;
	}

	/* Iterators */
	iterator begin() const { return iterator(_bottom->_next); }
	iterator end() const { return iterator(_top); }

	const_iterator const_begin() const { return const_iterator(_bottom->_next); }
	const_iterator const_end() const { return const_iterator(_top); }

	/* Destructor */
	~Linked()
	{
		//Delete all center nodes
		for (Node* it = _bottom->_next; it != _top;)
		{
			Node* current = it;
			it = it->_next;
			delete current;
		}
		//Delete top and bottom ghost nodes
		delete _top;
		delete _bottom;
	}

};

#endif

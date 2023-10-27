#pragma once
#include<assert.h>

namespace ysf
{
	template<class T>
	struct list_node
	{
		T _data;
		list_node<T>* next;
		list_node<T>* prev;

		list_node(const T& x = T())
			:_data(x)
			,next(nullptr)
			,prev(nullptr)
	};

	template<class T,class Ref,class Ptr>
	struct __list_iterator
	{
		typedef list_node<T> Node;
		typedef __list_iterator<T, Ref, Ptr> iterator;

		//下面这几个没什么用
		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef ptrdiff_t difference_type;

		Node* _node;

		__list_iterator(Node *node)
			:_node(node)
		{}

		bool operator !=(const iterator& it)const
		{
			return _node != it._node;
		}

		bool operator ==(const iterator& it)const
		{
			return _node == it._node;
		}

		Ref operator*()
		{
			return _node->_data;
		}
		Ptr operator->()
		{
			return &(operator*());
		}
		iterator& operator++()
		{
			_node = _node->next;
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp(*this);
			_node = _node->next;
			return tmp;
		}
		iterator& operator--()
		{
			_node = _node->_prev;
			return *this;
		}
		iterator operator--(int)
		{
			iterator tmp(*this);
			_node = _node->_prev;
			return tmp;
		}
	};

	template<class T>
	class list {
		typedef list_node<T> Node;
	public:
		typedef __list_iterator<T, T&, T*> iterator;
		typedef __list_iterator<T, const T&, const T*> const_iterator;

		const_iterator begin() const
		{
			return const_iterator(_head->_next);
		}

		const_iterator end() const
		{
			return const_iterator(_head);
		}

		iterator begin()
		{
			return const_iterator(_head->_next);
		}
		iterator end()
		{
			return iterator(_head);
		}

		void empty_init()
		{
			_head = new Node;
			_head->next = _head;
			_head->prev = _head;
		}

		list()
		{
			empty_init();
		}
		template<class InputIterator>
		list(InputIterator first, InputIterator end)
		{
			empty_init();
			while (first != end)
			{
				push_back(*first);
				++first;
			}
		}

		list(const list<T>lt)
		{
			empty_init();
			list<T> tmp(lt.begin(), lt.end());
			swap(tmp);
		}
		list<T> operator==(list<T> lt)
		{
			swap(lt);
			return *this;
		}

		void clear()
		{
			iterator it = begin();
			while (it != begin)
			{
				erase(it);
			}
		}

		~list()
		{
			clear();
			delete _head;
			_head = nullptr;
		}


		void swap(list<T>& x)
		{
			std::swap(_head, x._head);
		}


		void push_back(const T&x)
		{
			insert(end(), x);
		}

		void push_front(const T& x)
		{
			insert(begin(), x);
		}
		void pop_back()
		{
			erase(--end());
		}
		void pop_front()
		{
			erase(begin());
		}
		iterator insert(iterator pos, const T& x)
		{
			Node* cur = pos._node;
			Node* prev = cur->prev;
			Node* newnode = new Node(x);

			prev->next = newnode;
			newnode->next = cur;
			newnode->prev = prev;
			cur->prev = newnode;

			return iterator(newnode);
		}

		iterator erase(iterator pos)
		{
			assert(pos != end());

			Node* cur = pos._node;
			Node* prev = cur->prev;
			Node* next = cur->next;

			prev->next = next;
			next->prev = prev;
			delete cur;
			return iterator(next);
		}


	private:
		Node* _head;
	};

}
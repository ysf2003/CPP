#pragma once
#include<assert.h>

namespace ysf
{
	template<class T>
	class vector {
	public:
		typedef T* iterator;//模板定义迭代器
		typedef const T* const_iterator;//const迭代器

		//-------------------------------迭代器----------------------------
		iterator begin()
		{
			return _start;
		}

		iterator end()
		{
			return _finish;
		}

		const_iterator begin()const
		{
			return _start;
		}
		const_iterator end()const
		{
			return _finish;
		}

		//---------------------------------构造函数--------------------
		vector()//无参构造
			:_start(nullptr)
			,_finish(nullptr)
			,_end_of_storage(nullptr)
		{}


		//传统写法
		/*
		vector(const vector<T>& v)
		{
			_start = new[v.size()];
			//memcpy(_start, v._start, sizeof(T) * v.size());memcpy是按值拷贝，如果vector<T>T是自定义类型就会出现浅拷贝
			for (size_t i = 0; i < v.size(); ++i)
			{
				_start = v._start[i];
			}
			_finish = _start + v.size();
			_end_of_storage = _start + v.size();
		}*/

		//传统写法2
		/*
		vector(const vector<T>& v)
			:_start(nullptr)
			,_finish(nullptr)
			,_end_of_storage(nullptr)
		{
			reserve(v.size());
			for (const auto &e : v)
			{
				push_back(e);
			}
		}
		*/

		//构造函数1，用n个T来初始化
		vector(size_t n, const T& val = T())
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			reserve(n);
			for (size_t i = 0; i < n; ++i)
			{
				push_back(val);
			}
		}
		//对上面的重载，防止传两个int类型的时候size_t要隐式类型转换，不会优先匹配上面那个函数，导致用了其他函数报错
		vector(int n, const T& val = T())
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			reserve(n);
			for (int i = 0; i < n; ++i)
			{
				push_back(val);
			}
		}

		//构造函数2，用一段迭代区间初始化
		template<class InputIterator>
		vector(InputIterator first, InputIterator end)
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			while (first != end)
			{
				push_back(*first);
				++first;
			}
		}
		//vector的swap函数
		void swap(vector<T>&v)
		{
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_end_of_storage, v._end_of_storage);
		}
		//构造函数现代写法（构造函数3--拷贝构造）
		vector(const vector<T>& v)
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			vector<T> tmp(v.begin(), v.end());
			swap(tmp);
		}
		//赋值操作，需要用到拷贝构造，vector<T> v是传值传参，传值是拷贝构造(深拷贝)，然后swap，最后参数出了作用域直接销毁
		vector<T>& operator=(vector<T> v)
		{
			swap(v);
			return *this;
		}

		//----------------------------------析构函数------------------
		~vector()
		{
			delete[]_start;
			_finish = _end_of_storage = _start = nullptr;
		}
		//---------------------------------容量操作----------------------------
		size_t capacity()
		{
			return _end_of_storage - _start;
		}

		size_t size() 
		{
			return _finish - _start;
		}



		//----------------------------修改操作--------------------
	    //reverse开n个空间
		void reserve(size_t n)
		{
			if (n > capacity())
			{
				size_t sz = size();
				T* tmp = new T[n];
				if (_start)
				{
					//memcpy(tmp, _start, sizeof(T) * sz);不能用memcpy
					for (size_t i = 0; i < sz; ++i)
					{
						tmp[i] = _start[i];
					}
					delete[]_start;
				}
				_start = tmp;
				_finish = _start + sz;
				_end_of_storage = _start + n;
		   }
		}

		void resize(size_t n, const T& val = T())
		{
			if (n > capacity())
			{
				reserve(n);
			}

			if (n > size())
			{
				while (_finish < _start + n)
				{
					*_finish = val;
					++_finish;
				}
			}
			else
			{
				_finish = _start + n;
			}
		}


		//尾插
		void push_back(const T& x)
		{
			/*
			if (_finish = _end_of_storage)
			{
				reserve(capacity() == 0 ? 4 : capacity() * 2)
			}
			*_finish = x;
			_finish++;
			*/
			insert(end(), x);
		}
		//尾删
		void pop_back()
		{
			assert(_finish > _start);
				--_finish;
		}
		//返回删除添加的元素的那个位置
		iterator insert(iterator pos, const T& x)
		{
			assert(pos >= _start);
			assert(pos <= _finish);

			if (_finish == _end_of_storage)//如果没有数据就要扩容
			{
				size_t len = pos - _start;//记录pos的位置
				reserve(capacity() == 0 ? 4 : capacity() * 2);
				pos = _start + len;//更新pos的位置，防止pos访问已经释放的旧空间
			}
			iterator end = _finish - 1;
			while (end >= pos)
			{
				*(end + 1) = *end;
				end--;
			}
			*pos = x;
			++_finish;

			return pos;
		}
		//返回删除的下一个位置
		iterator erase(iterator pos)
		{
			assert(pos >= _start);
			assert(pos <= _finish);

			iterator begin = pos + 1;
			while (begin < _finish)
			{
				*(begin - 1) = *begin;
				++begin;
			}
			--_finish;
			return pos;
		}

		T& front()
		{
			assert(size() > 0);

			return *_start;
		}

		T& back()
		{
			assert(size() > 0);

			return *(_finish - 1);
		}


		//---------------------------访问遍历---------------------
		const T& operator[](size_t pos) const
		{
			assert(pos < size());
			return _start[pos];
		}

		T& operator[](size_t pos)
		{
			assert(pos < size());

			return _start[pos];
		}

	private:
		iterator _start;
		iterator _finish;
		iterator _end_of_storage;



	};


}
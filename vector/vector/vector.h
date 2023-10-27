#pragma once
#include<assert.h>

namespace ysf
{
	template<class T>
	class vector {
	public:
		typedef T* iterator;//ģ�嶨�������
		typedef const T* const_iterator;//const������

		//-------------------------------������----------------------------
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

		//---------------------------------���캯��--------------------
		vector()//�޲ι���
			:_start(nullptr)
			,_finish(nullptr)
			,_end_of_storage(nullptr)
		{}


		//��ͳд��
		/*
		vector(const vector<T>& v)
		{
			_start = new[v.size()];
			//memcpy(_start, v._start, sizeof(T) * v.size());memcpy�ǰ�ֵ���������vector<T>T���Զ������;ͻ����ǳ����
			for (size_t i = 0; i < v.size(); ++i)
			{
				_start = v._start[i];
			}
			_finish = _start + v.size();
			_end_of_storage = _start + v.size();
		}*/

		//��ͳд��2
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

		//���캯��1����n��T����ʼ��
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
		//����������أ���ֹ������int���͵�ʱ��size_tҪ��ʽ����ת������������ƥ�������Ǹ���������������������������
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

		//���캯��2����һ�ε��������ʼ��
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
		//vector��swap����
		void swap(vector<T>&v)
		{
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_end_of_storage, v._end_of_storage);
		}
		//���캯���ִ�д�������캯��3--�������죩
		vector(const vector<T>& v)
			:_start(nullptr)
			, _finish(nullptr)
			, _end_of_storage(nullptr)
		{
			vector<T> tmp(v.begin(), v.end());
			swap(tmp);
		}
		//��ֵ��������Ҫ�õ��������죬vector<T> v�Ǵ�ֵ���Σ���ֵ�ǿ�������(���)��Ȼ��swap������������������ֱ������
		vector<T>& operator=(vector<T> v)
		{
			swap(v);
			return *this;
		}

		//----------------------------------��������------------------
		~vector()
		{
			delete[]_start;
			_finish = _end_of_storage = _start = nullptr;
		}
		//---------------------------------��������----------------------------
		size_t capacity()
		{
			return _end_of_storage - _start;
		}

		size_t size() 
		{
			return _finish - _start;
		}



		//----------------------------�޸Ĳ���--------------------
	    //reverse��n���ռ�
		void reserve(size_t n)
		{
			if (n > capacity())
			{
				size_t sz = size();
				T* tmp = new T[n];
				if (_start)
				{
					//memcpy(tmp, _start, sizeof(T) * sz);������memcpy
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


		//β��
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
		//βɾ
		void pop_back()
		{
			assert(_finish > _start);
				--_finish;
		}
		//����ɾ����ӵ�Ԫ�ص��Ǹ�λ��
		iterator insert(iterator pos, const T& x)
		{
			assert(pos >= _start);
			assert(pos <= _finish);

			if (_finish == _end_of_storage)//���û�����ݾ�Ҫ����
			{
				size_t len = pos - _start;//��¼pos��λ��
				reserve(capacity() == 0 ? 4 : capacity() * 2);
				pos = _start + len;//����pos��λ�ã���ֹpos�����Ѿ��ͷŵľɿռ�
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
		//����ɾ������һ��λ��
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


		//---------------------------���ʱ���---------------------
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
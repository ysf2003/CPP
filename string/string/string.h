#pragma once
#include <assert.h>
#include<iostream>
using namespace std;
namespace ysf
{
	class string
	{

	public:
		//---------------------------迭代器---------------------------
		typedef char* iterator;
		iterator begin()
		{
			return _str;
		}

		iterator end()
		{
			return _str + _size;
		}

		//---------------------------构造函数---------------------------

		//可以直接和下面的构造函数合并，给一个全缺省的构造函数
		/*string()
			:_str(new char[1])//不能直接用nullptr来初始化，当用c_str输出的时候会解引用空指针
			, _size(0)
			, _capacity(0)
		{
			_str[0] = '\0';
		}*/
		string(const char* str = "")//或者直接=""，给空默认会给一个\0,这种方式相当于"\0\0"
			/* :_str(new char[strlen(str) + 1])
			,_size(strlen(str))
			,_capacity(strlen(str))  */
			//strlen是O(n)的需要改进
			
			/* :_size(strlen(str))
			,_capacity(_size)
			,_str(new char[_capacity + 1])*/
			//这种写法也有问题，初始化列表是根据定义的顺序初始化的会先初始化str然后是size，capacity
			//除非调换定义顺序，但是这增加了维护成本，严格绑定了定义的顺序
		{
			//干脆直接不用初始化列表，因为是内置类型也不需要调用其构造函数
			_size = strlen(str);
			_capacity = _size;
			_str = new char[_capacity + 1];
			strcpy(_str, str);
		}
		
		/*传统写法
		//深拷贝
		string(const string& s)
			:_str(new char[s._capacity + 1])
			,_capacity(s._capacity)
			,_size(s._size)
		{
			strcpy(_str, s._str);
		}

		//s1=s2,s1=s1
		string& operator=(const string& s)
		{
			if (this != &s)//自己给自己赋值就直接返回*this
			{
				//delete[]_str;
				//_str = new char[s._capacity + 1];
				//strcpy(_str, s._str);
				//下面这种写法如果new失败了不会破坏原本的数据没有直接delete_str
				char* tmp = new char[s._capacity + 1];
				strcpy(tmp, s._str);
				delete[]_str;
				_str = tmp;
				_size = s._size;
				_capacity = s._capacity;
			}
			return *this;
		}
		*/
		//库里面的swap代价很大，内置类型还好，自定义类型就不能用库的swap了，而且还会报错
		void swap(string& tmp)
		{
			::swap(_str, tmp._str);
			::swap(_size, tmp._size);
			::swap(_capacity, tmp._capacity);
		}

		//拷贝构造现代写法
		string(const string& s)
			:_str(nullptr)
			,_size(0)
			,_capacity(0)
		{
			string tmp(s._str);//调用默认构造
			swap(tmp);//this->swap(tmp);出作用域tmp自动调用析构释放空间
		}
		//operate= 赋值现代写法
		/*string& operator=(string const& s)
		{
			if (this != &s)
			{
				//string tmp(s);
				string tmp(s._str);
				swap(tmp);
			}//tmp出了作用域会自动析构

			return *this;
		}*/

		//现代写法最简洁版本
		string& operator=(string s)//传值调用拷贝构造，拷贝构造是深拷贝
		{
			swap(s);
			return *this;
		}

		//-------------------------- - 析构函数-------------------------- -

		~string()//析构函数
		{
			delete[] _str;
			_str = nullptr;
			_size = _capacity = 0;
		}

		//-------------------------- - 容量操作-------------------------- -
		size_t size() const
		{
			return _size;
		}
		size_t capacity() const
		{
			return _capacity;
		}



		//-------------------------- - 修改操作-------------------------- -

		//以c的方式输出
		const char* c_str() const
		{
			return _str;
		}
		

		//operate[]
		const char& operator[](size_t pos) const
		{
			assert(pos < _size);

			return _str[pos];
		}

		char& operator[](size_t pos)
		{
			assert(pos < _size);

			return _str[pos];
		}


		//Operator+=
		string& operator+=(char ch)
		{
			push_back(ch);
			return *this;
		}
		string& operator+=(const char* str)
		{
			append(str);
			return *this;
		}
		//初始化
		void resize(size_t n, char ch = '\0')
		{
			if (n > _size) {
				reserve(n);
				for (size_t i = _size; i < n;++i)
				{
					_str[i] = ch;
				}
				_str[n] = '\0';
				_size = n;
			}
			else
			{
				_str[n] = '\0';
				_size = n;
			}
		}


		//reserve开空间
		void reserve(size_t n)
		{
			if (n > _capacity)
			{
				char* tmp = new char[n+1];
				strcpy(tmp,_str);
				delete _str;
				_str = tmp;
				_capacity = n;
			}
		}
		//尾插
		void push_back(char ch)//可以复用insert
		{
			if (_size == _capacity)
			{
				reserve(_capacity == 0 ? 4 : _capacity * 2);
			}
			_str[_size] = ch;
			++_size;
			_str[_size] = '\0';
			//复用insert(_size,ch);
		}
		//尾插字符串
		void append(const char* str)
		{
			size_t len = strlen(str);

			if (_size + len > _capacity)
			{
				reserve(_size + len);
			}
			strcpy(_str + _size, str);
			_size += len;

		}
		//尾插n个字符
		void append(size_t n, char ch)
		{
			reserve(_size+n);
			for (size_t i = 0; i < n; ++i)
			{
				push_back(ch);
			}
			
		}

		//insert
		string& insert(size_t pos, char ch)
		{
			assert(pos <= _size);

			if (_size == _capacity)
			{
				reserve(_capacity == 0 ? 4 : _capacity * 2);
			}
			size_t end = _size + 1;
			while (end > pos)
			{
				_str[end] = _str[end - 1];
				--end;
			}
			_str[pos] = ch;
			++_size;
			return *this;
		}

		string& insert(size_t pos, const char * str)
		{
			assert(pos <= _size);
			size_t len = strlen(str);
			if (_size + len > _capacity)
			{
				reserve(_size + len);
			}
			size_t end = _size + len ;
			while (end >= pos + len)
			{
				_str[end] = _str[end - len];
				--end;
			}

			strncpy(_str + pos, str, len);
			_size += len;

			return *this;
		}

		void erase(size_t pos, size_t len = npos)//-1是整形的最大值，size_t中没有负数是二进制8个1
		{
			assert(pos < _size);
			if (len == npos || pos + len >= _size)
			{
				_str[pos] = '\0';
				_size = pos;
			}
			else
			{
				strcpy(_str + pos, _str + pos + len);
				_size -= len;
			}
		}
		void clear()
		{
			_str[0] = '\0';
			_size = 0;
		}

		size_t find(char ch, size_t pos = 0)
		{
			assert(pos < _size);
			for (size_t i = pos; i < _size; ++i)
			{
				if (ch == _str[i]);
				return i;
		    }
			return npos;
		}
		size_t find(const char* sub, size_t pos = 0) const
		{
			assert(sub);
			assert(pos < _size);

			//子串匹配可以用kmp/bm算法
			const char* ptr = strstr(_str + pos, sub);//C语言strstr是暴力匹配
			if (ptr = nullptr) {
					return npos;
			}
			else {
				return ptr - _str;
			}

		}
		//从pos位置开始取字符
		string substr(size_t pos, size_t len = npos) const
		{
			assert(pos < _size);
			size_t realLen = len;
			if (len == npos || pos + len > _size)
			{	
				realLen = _size - pos;
			}
			string sub;
			for (size_t i = 0; i < realLen;++i)
			{
				 sub+= _str[pos + i];
			}
			return sub;
		}



		//比较字符串大小
		bool operator>(const string& s) const
		{
			return strcmp(_str, s._str) > 0;
		}
		bool operator>=(const string& s) const
		{
			return (*this > s) || (*this == s);
		}
		bool operator==(const string& s) const
		{
			return strcmp(_str, s._str) == 0;
		}
		bool operator<(const string& s) const
		{
			return !(*this >= s);
		}
		bool operator<=(const string& s) const
		{
			return !(*this >= s);
		}
		bool operator!=(const string& s) const
		{
			return !(*this == s);
		}

		
	private:
		//VS下有一个char buff[16];如果字符小于16个，字符串存在buff数组中，
		//如果>=16 存在_str申请的堆空间上。
		char* _str;
		size_t _size;
		size_t _capacity;
        //const 静态可以直接定义 C++特例
	public:
		const static size_t npos = -1;
	};
	ostream& operator<<(ostream& out, const string& s)
	{
		for (size_t i = 0; i < s.size(); ++i)
		{
			out << s[i];
		}
		return out;	
	}

	istream& operator>>(istream& in, string& s)
	{
		s.clear();
		char ch;
		ch = in.get();
		const size_t N = 32;
		char buff[N];//出了作用域就销毁
		size_t i = 0;
		//相当于一组32个，可以解决输入会慢慢扩容(0,4,8,16...)的问题
		while (ch != ' '&& ch != '\n')
		{
			buff[i++] = ch;
			
			if (i == N - 1)//一组32，满了重新从0开始
			{
				buff[i] = '\0';
				s += buff;
				i = 0;
			}
			ch = in.get();
		}
		buff[i] = '\0';
		s += buff;
		return in;
	}

}


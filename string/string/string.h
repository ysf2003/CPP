#pragma once
#include <assert.h>
#include<iostream>
using namespace std;
namespace ysf
{
	class string
	{

	public:
		//---------------------------������---------------------------
		typedef char* iterator;
		iterator begin()
		{
			return _str;
		}

		iterator end()
		{
			return _str + _size;
		}

		//---------------------------���캯��---------------------------

		//����ֱ�Ӻ�����Ĺ��캯���ϲ�����һ��ȫȱʡ�Ĺ��캯��
		/*string()
			:_str(new char[1])//����ֱ����nullptr����ʼ��������c_str�����ʱ�������ÿ�ָ��
			, _size(0)
			, _capacity(0)
		{
			_str[0] = '\0';
		}*/
		string(const char* str = "")//����ֱ��=""������Ĭ�ϻ��һ��\0,���ַ�ʽ�൱��"\0\0"
			/* :_str(new char[strlen(str) + 1])
			,_size(strlen(str))
			,_capacity(strlen(str))  */
			//strlen��O(n)����Ҫ�Ľ�
			
			/* :_size(strlen(str))
			,_capacity(_size)
			,_str(new char[_capacity + 1])*/
			//����д��Ҳ�����⣬��ʼ���б��Ǹ��ݶ����˳���ʼ���Ļ��ȳ�ʼ��strȻ����size��capacity
			//���ǵ�������˳�򣬵�����������ά���ɱ����ϸ���˶����˳��
		{
			//�ɴ�ֱ�Ӳ��ó�ʼ���б���Ϊ����������Ҳ����Ҫ�����乹�캯��
			_size = strlen(str);
			_capacity = _size;
			_str = new char[_capacity + 1];
			strcpy(_str, str);
		}
		
		/*��ͳд��
		//���
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
			if (this != &s)//�Լ����Լ���ֵ��ֱ�ӷ���*this
			{
				//delete[]_str;
				//_str = new char[s._capacity + 1];
				//strcpy(_str, s._str);
				//��������д�����newʧ���˲����ƻ�ԭ��������û��ֱ��delete_str
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
		//�������swap���ۺܴ��������ͻ��ã��Զ������;Ͳ����ÿ��swap�ˣ����һ��ᱨ��
		void swap(string& tmp)
		{
			::swap(_str, tmp._str);
			::swap(_size, tmp._size);
			::swap(_capacity, tmp._capacity);
		}

		//���������ִ�д��
		string(const string& s)
			:_str(nullptr)
			,_size(0)
			,_capacity(0)
		{
			string tmp(s._str);//����Ĭ�Ϲ���
			swap(tmp);//this->swap(tmp);��������tmp�Զ����������ͷſռ�
		}
		//operate= ��ֵ�ִ�д��
		/*string& operator=(string const& s)
		{
			if (this != &s)
			{
				//string tmp(s);
				string tmp(s._str);
				swap(tmp);
			}//tmp������������Զ�����

			return *this;
		}*/

		//�ִ�д������汾
		string& operator=(string s)//��ֵ���ÿ������죬�������������
		{
			swap(s);
			return *this;
		}

		//-------------------------- - ��������-------------------------- -

		~string()//��������
		{
			delete[] _str;
			_str = nullptr;
			_size = _capacity = 0;
		}

		//-------------------------- - ��������-------------------------- -
		size_t size() const
		{
			return _size;
		}
		size_t capacity() const
		{
			return _capacity;
		}



		//-------------------------- - �޸Ĳ���-------------------------- -

		//��c�ķ�ʽ���
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
		//��ʼ��
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


		//reserve���ռ�
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
		//β��
		void push_back(char ch)//���Ը���insert
		{
			if (_size == _capacity)
			{
				reserve(_capacity == 0 ? 4 : _capacity * 2);
			}
			_str[_size] = ch;
			++_size;
			_str[_size] = '\0';
			//����insert(_size,ch);
		}
		//β���ַ���
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
		//β��n���ַ�
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

		void erase(size_t pos, size_t len = npos)//-1�����ε����ֵ��size_t��û�и����Ƕ�����8��1
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

			//�Ӵ�ƥ�������kmp/bm�㷨
			const char* ptr = strstr(_str + pos, sub);//C����strstr�Ǳ���ƥ��
			if (ptr = nullptr) {
					return npos;
			}
			else {
				return ptr - _str;
			}

		}
		//��posλ�ÿ�ʼȡ�ַ�
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



		//�Ƚ��ַ�����С
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
		//VS����һ��char buff[16];����ַ�С��16�����ַ�������buff�����У�
		//���>=16 ����_str����Ķѿռ��ϡ�
		char* _str;
		size_t _size;
		size_t _capacity;
        //const ��̬����ֱ�Ӷ��� C++����
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
		char buff[N];//���������������
		size_t i = 0;
		//�൱��һ��32�������Խ���������������(0,4,8,16...)������
		while (ch != ' '&& ch != '\n')
		{
			buff[i++] = ch;
			
			if (i == N - 1)//һ��32���������´�0��ʼ
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


#define  _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <assert.h>
#include<vector>
#include "vector.h"
using namespace std;
using namespace ysf;

int main()
{

	ysf::vector<int> v;
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);

	v.resize(10, 100);

	for (auto e : v)
	{
		cout << e << endl;
	}


	return 0;
}
#include "myallocator.h"
#include <vector>//空间配置器 毕竟是给容器使用的

using namespace std;


int main()
{
	vector<int, myallocator<int>>myvec;

	//接下来 就是进行测试使用了
	for (int i = 0; i < 10; ++i)
	{
		myvec.push_back(rand() % 100 + 1);
	}

	for (int val : myvec)
	{
		cout << val << endl;
	}
	return 0;
}




















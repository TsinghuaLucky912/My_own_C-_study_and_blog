
/**══════════════════════════════════╗
*作    者：songjinzhou                                                 ║
*CSND地址：https://blog.csdn.net/weixin_43949535                       ║
***GitHub：https://github.com/TsinghuaLucky912/My_own_C-_study_and_blog║
*═══════════════════════════════════╣
*创建时间：2019年9月1日11:34:01                                                            
*功能描述：                                                            
*                                                                      
*                                                                      
*═══════════════════════════════════╣
*结束时间: 2019年9月1日13:50:07                                                            
*═══════════════════════════════════╝
//                .-~~~~~~~~~-._       _.-~~~~~~~~~-.
//            __.'              ~.   .~              `.__
//          .'//              西南\./联大               \\`.
//        .'//                     |                     \\`.
//      .'// .-~"""""""~~~~-._     |     _,-~~~~"""""""~-. \\`.
//    .'//.-"                 `-.  |  .-'                 "-.\\`.
//  .'//______.============-..   \ | /   ..-============.______\\`.
//.'______________________________\|/______________________________`.
*/
#include <iostream>
#include <vector>
#include <iomanip>
#include <time.h>

using namespace std;

const int MAX = 15;

void Counting_sort1(vector<int>& src)
{
	int size = src.size();
	static int count = 0;//记录循环进入的次数
	int maxval = src[0];//记录数组中元素的最大值

	for (int i = 1; i < size; ++i)
	{
		if (src[i] > maxval)
			maxval = src[i];
	}

	//所以说 应该开辟的大小就是maxval+1
	vector<int>temp_src(maxval  + 1);//这是临时开辟的数组,全初始化为0

	for (int i = 0; i < size; ++i)
	{
		temp_src[src[i]]++;//统计 每个数据出现的次数，并把这个次数作为新记录数组的下标
	}

	//接下来就是反向处理数组，放置元素
	src.clear();

	for (int i = 0; i < temp_src.size(); ++i)
	{
		while (temp_src[i] != 0)
		{
			src.push_back(i);
			temp_src[i]--;
			count++;
		}
	}
	cout << "共进入循环次数：" << count << endl;
}

void Counting_sort(vector<int>& src)
{
	int size = src.size();
	static int count = 0;//记录循环进入的次数
	int maxval = src[0];//记录数组中元素的最大值

	for (int i = 1; i < size; ++i)
	{
		if (src[i] > maxval)
			maxval = src[i];
	}

	//所以说 应该开辟的大小就是maxval+1
	vector<int>temp_src(maxval + 1);//这是临时开辟的数组,全初始化为0

	//统计值为src[i]的个数;而temp_src[i]是等于i的元素个数
	for (int i = 0; i < size; ++i)
	{
		temp_src[src[i]]++;//统计 每个数据出现的次数，并把这个次数作为新记录数组的下标
	}
	for (int i = 1; i < temp_src.size(); ++i)
	{
		temp_src[i] += temp_src[i - 1];//小于当前数据元素的个数
	}
	//接下来就是反向处理数组，放置元素
	//需要再额外开辟一块和 src一样大小的数组空间
	vector<int>new_src;
	new_src.resize(size);
	int new_index_of_newsrc = 0;//新空间元素的下标
	for (int i = size - 1; i >= 0; --i)
	{
		new_index_of_newsrc = --temp_src[src[i]];
		new_src[new_index_of_newsrc] = src[i];
		count++;
	}

	src.clear();
	for (int i = 0; i < size; ++i)
	{
		src.push_back(new_src[i]);
	}
	
	cout << "共进入循环次数：" << count << endl;
}

int main()
{
	srand((unsigned)time(nullptr));
	vector<int>myvec;
	for (int i = 0; i < MAX; ++i)
	{
		myvec.push_back(rand() % 11);//这组数据的范围是：0---10
	}
	cout << "数列初始状态：";
	for (int val : myvec)
		cout << setw(2) << val << " ";
	cout << endl;
	cout << "/*-----------------------------------------------------*/" << endl;
	Counting_sort1(myvec);
	cout << "/*-----------------------------------------------------*/" << endl;
	cout << "数列最终状态：";
	for (int val : myvec)
		cout << setw(2) << val << " ";
	cout << endl;
	cout << "***************************************************************" << endl;

	int Array[] = { 2,5,3,0,2,3,0,3 };
	vector<int>myvec1(begin(Array), end(Array));
#if 0
	for (int i = 0; i < MAX; ++i)
	{
		myvec1.push_back(rand() % 11);//这组数据的范围是：0---10
	}
#endif 

	cout << "数列初始状态：";
	for (int val : myvec1)
		cout << setw(2) << val << " ";
	cout << endl;
	cout << "/*-----------------------------------------------------*/" << endl;
	Counting_sort(myvec1);
	cout << "/*-----------------------------------------------------*/" << endl;
	cout << "数列最终状态：";
	for (int val : myvec1)
		cout << setw(2) << val << " ";
	cout << endl;
	
	return 0;
}
/**
*备用注释：
*
*
*
*/

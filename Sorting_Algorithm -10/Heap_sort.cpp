
/**══════════════════════════════════╗
*作    者：songjinzhou                                                 ║
*CSND地址：https://blog.csdn.net/weixin_43949535                       ║
***GitHub：https://github.com/TsinghuaLucky912/My_own_C-_study_and_blog║
*═══════════════════════════════════╣
*创建时间：2019年8月31日16:21:41                                                            
*功能描述：                                                            
*                                                                      
*                                                                      
*═══════════════════════════════════╣
*结束时间: 2019年9月1日11:32:22                                                            
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
#include <algorithm>
#include <iomanip>
using namespace std;

//下面这是 主要进行调整操作的
void adjust_new_heap(vector<int>& src, int father,int last)
{
	for (int i = father * 2 + 1; i <= last; i=2*i+1)//last是最后可以进行调整到 的元素下标
	{
		if (i < last - 1 && src[i] < src[i + 1])//i是左孩子 且比右孩子小
		{
			++i;
		}
		if (src[i] > src[father])//这里 是处理孩子中 大者和父节点的交换
		{
			swap(src[i], src[father]);
			father = i;
		}
		else break;//都比父节点小  没必要向下 调整了
	}
}

void Heap_sort(vector<int>& src)
{
	int size = src.size();
	//数组的大小是size 则其下标为【0，size-1】

	//第一步：从最后一个父节点开始开始 与其子节点进行比较
	//其下标为 (size-1)/2
	for (int i = (size - 1) / 2; i >= 0; --i)//依次处理这个父节点，进而初始化大顶堆
	{
		adjust_new_heap(src, i,size - 1);
	}
	cout << "数列初次调整：";
	for (int val : src)
		cout << setw(2) << val << " ";
	cout << endl;
	cout << "/*-----------------------------------------------------*/" << endl;
	for (int i = 0; i < size - 1; ++i)
	{
		swap(src[0], src[size - 1 - i]);//这是swap交换值

		cout << "数列此时交换：";
		for (int val : src)
			cout << setw(2) << val << " ";
		cout << endl;

		adjust_new_heap(src, 0, size - 2 - i);

		cout << "数列此时调整：";
		for (int val : src)
			cout << setw(2) << val << " ";
		cout << endl;
	}
}

int main()
{
	int Array[] = { 12 ,34 ,6 ,21 ,4 ,33 ,45 ,53 ,15 ,5 };
	vector<int>myvec(begin(Array), end(Array));

	cout << "数列初始状态：";
	for (int val : myvec)
		cout << setw(2) << val << " ";
	cout << endl;
	cout << "/*-----------------------------------------------------*/" << endl;
	Heap_sort(myvec);
	cout << "/*-----------------------------------------------------*/" << endl;
	cout << "数列最终状态：";
	for (int val : myvec)
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

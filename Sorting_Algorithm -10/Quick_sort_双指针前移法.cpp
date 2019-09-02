
/**══════════════════════════════════╗
*作    者：songjinzhou                                                 ║
*CSND地址：https://blog.csdn.net/weixin_43949535                       ║
***GitHub：https://github.com/TsinghuaLucky912/My_own_C-_study_and_blog║
*═══════════════════════════════════╣
*创建时间：                                                            
*功能描述：                                                            
*                                                                      
*                                                                      
*═══════════════════════════════════╣
*结束时间:                                                             
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
#include <string>
#include <algorithm>
#include <time.h>
using namespace std;


static int k = 1;
// src[mid] <= src[low] <= src[high]
void resetlow_mid_high(vector<int>& src, int low, int high)//重置 三数取中
{
	int mid = (low + high) >> 1;
	if (src[mid] > src[low])
	{
		swap(src[mid], src[low]);
	}
	if (src[low] > src[high])
	{
		swap(src[low], src[high]);
	}
	if (src[mid] > src[high])
	{
		swap(src[mid], src[high]);
	}
}
void Straight_Insertion_Sort2(vector<int>& src)//使用的是两两交换前进
{
	int size = src.size();
	for (int i = 0; i < size - 1; ++i)
	{
		int j = i + 1;//取出无序序列的第一个元素
		int k = i;
		while (k >= 0 && src[j] < src[k])//这个k的比较 必须放前面
		{
			swap(src[k], src[j]);
			k--;
			j--;
		}
	}
	cout << "第" << k++ << "次的, 结束结果为：";
	for (int val : src)
		cout << setw(2) << val << " ";

	cout << endl;
}

int one_Partition(vector<int>& src, int low, int high)// 左右交换法
{
	int l = low, h = high;
	int pivotkey = src[low];//这次把最左边的数据 作为基准数
	int val_key = l;
	int count_k = 0;

	while (l < h)
	{
		while (l<h && src[h] >= pivotkey)
		{
			h--;
		}
		//在这里 就找到了 自最右边开始的 第一个小于pivotkey值的 数据

		//然后接下来 就在给定数列 最左边开始找带一个大于 pivotkey值的 数据
		while (l < h && src[l] <= pivotkey)
		{
			l++;
		}
		count_k++;
		swap(src[l], src[h]);
	}
	//最后把pivotkey值 和中间集合下标数据 进行交换
	swap(src[l], src[val_key]);
	count_k++;

	//打印一下每一次的  一次划分结束时的结果：
	cout << "第" << k++ << "次的一次划分,经过了" << count_k << "次的交换。 结束结果为：";
	for (int val : src)
		cout << setw(2) << val << " ";

	cout << endl;
	return l;//把这个下标 返回，作为下一次递归的base中间线
}

void Quick_sort(vector<int>& src, int low, int high)
{
	int base = one_Partition(src, low, high);//双指针前移法 实现
	if (high - low + 1 < 5)  // 当整个序列的大小high-low+1<5时，采用直接插入排序
	{
		Straight_Insertion_Sort2(src);
		return;
	}
	else
	{
		if (low + 1 < base)
			Quick_sort(src, low, base - 1);//递归左边

		if (base + 1 < high)
			Quick_sort(src, base + 1, high);//递归右边
	}
}



int main()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };
	int Array2[] = { 49,38,65,97,76,13,27,49 };
#if 0
	vector<int>myvec(begin(Array), end(Array));
	Quick_sort(myvec, 0, myvec.size() - 1);
	cout << "排序Quick_sort结束后，最终结果：";
	for (int val : myvec)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	cout << "**********************************" << endl;
#endif
	int Array3[] = { 8,7,6,5,4,3,2,1 };
	vector<int>myvec2(begin(Array3), end(Array3));
	cout << "排序Quick_sort结束前，最终结果：              ";
	for (int val : myvec2)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	Quick_sort(myvec2, 0, myvec2.size() - 1);
	cout << "排序Quick_sort结束前，最终结果：              ";
	for (int val : myvec2)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	return 0;
}
/**
*备用注释：
*
*
*
*/

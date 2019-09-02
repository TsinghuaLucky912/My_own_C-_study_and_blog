
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
	cout << "第" << k++ << "次的, 结束结果为（直接插入排序）：         ";
	for (int val : src)
		cout << setw(2) << val << " ";

	cout << endl;
}



//下面是 一次划分 处理和基准值pivotkey重复数据的
int one_Partition(vector<int>& src, int low, int high)// 左右交换法
{
	int l = low, h = high;
	resetlow_mid_high(src, low, high);//三数取中 得到基准数

	int pivotkey = src[low];
	int val_key = l;
	int count_k = 0;

	while (l < h)
	{
		while (l < h && src[h] >= pivotkey)
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
	cout << "第" << k++ << "次的一次划分,经过了" << count_k 
		<< "次的交换。 结束结果为：";
	for (int val : src)
		cout << setw(2) << val << " ";

	cout << endl;
	return l;//把这个下标 返回，作为下一次递归的base中间线
}

void gather_same_pivotkey(vector<int>& src, int low, int base, int high,
										int* same_pivotkey_left, int* same_pivotkey_right)
{
	int this_left = base - 1;//base 是上一次 一次划分返还的中间线位置下标
	if (low < high)
	{
		for (int i = base - 1; i >= low; --i)
		{
			if (src[i] == src[base] && i != this_left)
			{
				swap(src[i], src[this_left]);
				this_left--;
			}
		}
		if (this_left >= low && src[this_left] == src[base])
			* same_pivotkey_left = this_left;
		else *same_pivotkey_left = this_left + 1;

		//////////////////////////////////////////////////////////////
		//接下来 处理base的 右边数据
		int this_right = base + 1;
		for (int i = base + 1; i <= high; ++i)
		{
			if (src[i] == src[base] && i != this_right)
			{
				swap(src[i], src[this_right]);
				this_right++;
			}
		}
		if (this_right <= high && src[this_right] == src[base])
			* same_pivotkey_right = this_right;
		else *same_pivotkey_right = this_right - 1;
	}
}
void Quick_sort(vector<int>& src, int low, int high)
{
#if 0
	if (high - low + 1 < 10)  // 当整个序列的大小high-low+1<10时，采用直接插入排序
	{
		Straight_Insertion_Sort2(src);
		return;
	}
#endif
	int base = one_Partition(src, low, high);

	//和基准数相同的 数值区间的左右下标
	int same_pivotkey_left = 0, same_pivotkey_right = 0;
	
	//将一次划分之后的  和pivotkey相同的数据 都集中到base的附近
	//即：【same_pivotkey_left，same_pivotkey_right】
	gather_same_pivotkey(src, low, base, high, &same_pivotkey_left, &same_pivotkey_right);

	//接下来 处理的时候：只需要从same_pivotkey_left的左边开始
	if (low + 1 < same_pivotkey_left)
		Quick_sort(src, low, same_pivotkey_left - 1);//递归左边
	//和只需要从same_pivotkey_right的右边开始
	if (same_pivotkey_right + 1 < high)
		Quick_sort(src, same_pivotkey_right + 1, high);//递归右边
}



int main()
{
	int Array[] = { 1, 4, 6 ,7 ,6, 6, 7, 6, 8, 6 };
	vector<int>myvec(begin(Array), end(Array));
	
	cout << "排序Quick_sort结束前，初始状态：              ";
	for (int val : myvec)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	Quick_sort(myvec, 0, myvec.size() - 1);
	cout << "排序Quick_sort结束后，最终结果：              ";
	for (int val : myvec)
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


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
#include <algorithm>
using namespace std;

int one_Partition(vector<int>& src, int low, int high)
{
	int l = low, h = high;
	int pivotkey = src[l];//把这段数据的第一个数据 作为基准值
	int val_key = l;
	while (l < h)
	{
		while (l < h && src[h] >= pivotkey)//这种情况下 必须先得从最右边开始
		{
			h--;
		}
		//在这里 就找到了 自最右边开始的 第一个小于pivotkey值的 数据

		//然后接下来 就在给定数列 最左边开始找带一个大于 pivotkey值的 数据
		while (l < h && src[l] <= pivotkey)
		{
			l++;
		}
		swap(src[l], src[h]);//把pivotkey值 两边的数据进行 swap
	}
	swap(src[l], src[val_key]);//最后把pivotkey值 和中间集合下标数据 进行交换
	return l;//把这个下标 返回，作为下一次递归的base中间线
}

void Quick_sort(vector<int>& src, int low, int high)
{
	int base = one_Partition(src, low, high);//左右交换法 实现

	if (low + 1 < base)
		Quick_sort(src, low, base - 1);//递归左边

	if (base + 1 < high)
		Quick_sort(src, base + 1, high);//递归右边
}

int main()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };
	int Array2[] = { 49,38,65,97,76,13,27,49 };

	vector<int>myvec(begin(Array), end(Array));
	Quick_sort(myvec, 0, myvec.size() - 1);
	cout << "排序Quick_sort结束后，最终结果：";
	for (int val : myvec)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	cout << "**********************************" << endl;

	vector<int>myvec2(begin(Array2), end(Array2));
	Quick_sort(myvec2, 0, myvec2.size() - 1);
	cout << "排序Quick_sort结束后，最终结果：";
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

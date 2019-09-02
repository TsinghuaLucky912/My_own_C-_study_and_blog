
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

int one_Partition(vector<int>& src, int low, int high)
{
	static int n = 1;//记录次数的
	int l = low, h = high;
	int pivotkey = src[l];//把这段数据的第一个数据 作为基准值

	while (l < h)
	{
		while (l < h  && src[h]>= pivotkey)
		{
			h--;
		}
		swap(src[l], src[h]);//把pivotkey填到src【h】里面，把src【l】这个坑挖好了

		while (l < h && src[l] <= pivotkey)
		{
			l++;
		}
		swap(src[l], src[h]);//把pivotkey填到src【l】里面，把src【h】这个坑挖好了
	}
	//src[l] = pivotkey;//这个是把pivotkey 放到最后一个位置
	//但是我感觉上面的这句代码 也可以不用写。因为我做的是 交换数据，本来就在那里了

	cout << "第" << n++ << "次调整结束后，结果：";
	for (int val : src)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	return l;//把这个下标 返回，作为下一次递归的base中间线
}

void Quick_sort(vector<int>& src,int low,int high)
{
	int base = one_Partition(src, low, high);

	if (low + 1 < base)
		Quick_sort(src, low, base - 1);//递归左边

	if (base + 1 < high)
		Quick_sort(src, base + 1, high);//递归右边
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

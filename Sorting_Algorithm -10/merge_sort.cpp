#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <assert.h>
using namespace std;

//一直向下递归 左右两侧都return的时候，进行调整 完了之后返回上一层 继续
void Merge_sort_Recur(vector<int>& src, int low, int high)
{
	if (low == high)//把检查边缘问题放在递归循环的外侧
		return;
	int mid = (high - low) / 2 + low;

	Merge_sort_Recur(src, low, mid);
	Merge_sort_Recur(src, mid + 1, high);

	//接下来就是进行归并了（递归开始返回了）
	vector<int> tem_vec;//这是起中间过渡的作用，放有序部分 数据
	tem_vec.resize(high - low + 1);

	int i = low, j = mid + 1, k = 0;//k是临时数组的下标
	while (i <= mid && j <= high)
	{
		if (src[i] > src[j])
		{
			tem_vec[k++] = src[j++];
		}
		else
		{
			tem_vec[k++] = src[i++];
		}
	}
	while (i <= mid)
	{
		tem_vec[k++] = src[i++];
	}
	while (j <= high)
	{
		tem_vec[k++] = src[j++];
	}

	//数据次序调整结束了
	//处理结束了  得把有序数据还给src
	k = 0;
	for (int i = low; i <= high; ++i)
	{
		src[i] = tem_vec[k++];
	}
}

void merge_sort1(vector<int>& src)//递归版本
{
	int size = src.size();
	if (size <= 1)
		return;
	Merge_sort_Recur(src, 0, size - 1);
}


void MergeSortNonRecursion(vector<int>& src)
{
	int size = src.size();
	if (size <= 1)
		return;
	vector<int> tem_vec;//这是起中间过渡的作用，放有序部分 数据
	tem_vec.resize(size);

	//逐级上升，第一次比较2个，第二次比较4个，第三次比较8个。。。
	for (int level = 1; level < size; level *= 2)
	{
		int left_min, left_max, right_min, right_max;
		//每次都是从数组的头元素开始  i=0
		for (left_min = 0; left_min < size - level; left_min = right_max)
		{
			right_min = left_max = left_min + level;
			right_max = left_max + level;

			//右边的下标最大值只能为size
			if (right_max > size)
			{
				right_max = size;
			}

			int k = 0;//k是临时数组的下标
			//如果左边的数据还没达到分割线且右边的数组没到达分割线，开始循环
			while (left_min < left_max && right_min < right_max)
			{
				if (src[left_min] < src[right_min])
				{
					tem_vec[k++] = src[left_min++];

				}
				else
				{
					tem_vec[k++] = src[right_min++];
				}
			}
			//上面循环结束的条件有两个，如果是左边的游标尚未到达，那么需要把
			//数组接回去，可能会有疑问，那如果右边的没到达呢，其实模拟一下就可以
			//知道，如果右边没到达，那么说明右边的数据比较大，这时也就不用移动位置了

			while (left_min < left_max)
			{
				//如果left_min小于left_max，说明现在左边的数据比较大
				//直接把它们接到数组的min之前就行
				src[--right_min] = src[--left_max];
			}
			while (k > 0)
			{
				//把排好序的那部分数组返回该k
				src[--right_min] = tem_vec[--k];
			}	
		}
	}
}

void merge_sort2(vector<int>& src)//非递归的实现
{
	MergeSortNonRecursion(src);
}

int main()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };

	int Array2[] = { 3,2,4,6,5 };
	vector<int>myvec(begin(Array), end(Array));
	merge_sort1(myvec);
	cout << "排序shell_sort1结束后，最终结果：";
	for (int val : myvec)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	cout << "**********************************" << endl;
	vector<int>myvec2(begin(Array), end(Array));
	merge_sort2(myvec2);
	cout << "排序shell_sort2结束后，最终结果：";
	for (int val : myvec2)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;

	return 0;
}
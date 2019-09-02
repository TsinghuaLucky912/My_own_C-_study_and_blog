#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;


//这个是直接插入排序
int Straight_Insertion_Sort1(vector<int>& src,int gap,int count)//使用的是数组后移，然后插入
{
	int size = src.size();
	for (int i = gap; i < size; ++i)
	{
		int j = i - gap;//取出无序序列的第一个元素
		int tempval = src[i];
		while (j >= 0 && tempval < src[j])//这个k的比较 必须放前面
		{
			count++;
			src[j + gap] = src[j];
			j -= gap;
		}
		src[j + gap] = tempval;
	}
	return count;
}
void shell_sort3(vector<int>& src)
{
	int size = src.size();
	int count = 0;
	int n = 0;
	for (int gap = size / 2; gap >= 1; gap /= 2)
	{
		count=Straight_Insertion_Sort1(src, gap, count);
		cout << "第" << setw(2) << n++ + 1 << "次排序后，结果为：";
		for (int k = 0; k <= size - 1; ++k)
		{
			cout << setw(2) << src[k] << " ";
		}
		cout << endl;
	}
	cout << "整个排序过程共进行了" << count << "次的移动" << endl;
}

//使用  简单的增量序列gap = {n/2 ,n/4, n/8 .....1}
void shell_sort1(vector<int>& src)
{
	int size = src.size();
	int count = 0;//计作移动的次数

	int n = 0;
	for (int gap = size / 2; gap >= 1; gap /= 2)
	{
		int i = gap;
		for (; i < size; ++i)
		{
			int j = i - gap;
			int temp = src[i];
			while (j >= 0 && src[j] > temp)
			{
				count++;
				src[j + gap] = src[j];
				j -= gap;
			}
			src[j + gap] = temp;
		}
		
		cout << "第" << setw(2) << n++ + 1 << "次排序后，结果为：";
		for (int k = 0; k <= size - 1; ++k)
		{
			cout << setw(2) << src[k] << " ";
		}
		cout << endl;
	}
	cout << "整个排序过程共进行了" << count << "次的移动" << endl;
}
//使用给定增量序列gap={5,3,1}
void shell_sort2(vector<int>& src)
{
	int size = src.size();
	int count = 0;//计作移动的次数

	int n = 0, m = 0;
	int gapArr[] = { 5,3,1 };
	for (int gap = gapArr[m]; m<sizeof(gapArr)/sizeof(gapArr[0]);gap=gapArr[m])
	{
		m++;
		int i = gap;
		for (; i < size; ++i)
		{
			int j = i - gap;
			int temp = src[i];
			while (j >= 0 && src[j] > temp)
			{
				count++;
				src[j + gap] = src[j];
				j -= gap;
			}
			src[j + gap] = temp;
		}

		cout << "第" << setw(2) << n++ + 1 << "次排序后，结果为：";
		for (int k = 0; k <= size - 1; ++k)
		{
			cout << setw(2) << src[k] << " ";
		}
		cout << endl;
	}
	cout << "整个排序过程共进行了" << count << "次的移动" << endl;
}
int main()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };

	vector<int>myvec(begin(Array), end(Array));
	shell_sort1(myvec);
	cout << "排序shell_sort1结束后，最终结果：";
	for (int val : myvec)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	cout << "**********************************" << endl;
	vector<int>myvec2(begin(Array), end(Array));
	shell_sort2(myvec2);
	cout << "排序shell_sort2结束后，最终结果：";
	for (int val : myvec2)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	cout << "**********************************" << endl;
	vector<int>myvec3(begin(Array), end(Array));
	shell_sort3(myvec3);
	cout << "排序shell_sort3结束后，最终结果：";
	for (int val : myvec3)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	return 0;
}


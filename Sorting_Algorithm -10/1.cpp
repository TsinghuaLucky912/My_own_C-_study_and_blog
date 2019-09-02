#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

void Straight_Insertion_Sort1(vector<int>& src)
{
	int size = src.size();
	for (int i = 0; i < size - 1; ++i)
	{
		int j = i + 1;//取出无序序列的第一个元素
		while (src[j] < src[i] && i >= 0)
		{
			swap(src[i], src[j]);
			i--;
			j--;
		}
		

		cout << "第" << setw(2) << i + 1 + 1 << "次排序后，结果为：";
		for (int k = 0; k <= size - 1; ++k)
		{
			cout << setw(2) << src[k] << " ";
		}
		cout << endl;

	}
	cout << "整个排序过程共进行了" << count << "次的交换" << endl;
}
void Straight_Insertion_Sort2(vector<int>& src)
{
	int size = src.size();
	int count = 0;//计作交换的次数
	for (int i = 0; i < size - 1; ++i)
	{
		int min = i;//默认最小值下标		
		for (int j = i + 1; j < size; ++j)
		{
			if (src[j] < src[min])
			{
				min = j;//记录一下 新的最小值下标
			}
		}
		if (min != i)//在这里进行预判
		{
			count++;
			std::swap(src[min], src[i]);
		}
		cout << "第" << setw(2) << i + 1 << "次排序后，结果为：";
		for (int k = 0; k <= size - 1; ++k)
		{
			cout << setw(2) << src[k] << " ";
		}
		cout << endl;
	}
	cout << "整个排序过程共进行了" << count << "次的交换" << endl;
}

int main()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };
	vector<int>myvec(begin(Array), end(Array));

	Straight_Insertion_Sort1(myvec);
	cout << "排序结束后，最终结果：";
	for (int val : myvec)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	cout << "*******************************************" << endl;
	vector<int>myvec2(begin(Array), end(Array));
	Straight_Insertion_Sort2(myvec2);

	cout << "排序结束后，最终结果：";
	for (int val : myvec2)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	return 0;
}


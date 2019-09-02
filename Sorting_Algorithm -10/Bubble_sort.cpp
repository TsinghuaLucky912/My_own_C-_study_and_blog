#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

void Bubble_sort1(vector<int>& src)
{
	int size = src.size();
	for (int i = 0; i < size - 1; ++i)
	{
		for (int j = 0; j < size - i - 1; ++j)
		{
			if (src[j] > src[j + 1])
			{
				int temp = src[j];
				src[j] = src[j + 1];
				src[j + 1] = temp;
			}
		}

		cout << "第" << setw(2) << i + 1 << "次排序后，结果为：";
		for (int k=0;k<=size-1;++k)
		{
			cout << setw(2) << src[k] << " ";
		}
		cout << endl;
	}
}
void Bubble_sort2(vector<int>& src)
{
	int size = src.size();
	for (int i = 0; i < size - 1; ++i)
	{
		int flag = true;//每趟循环开始 默认有序 不需要交换
		for (int j = 0; j < size - i - 1; ++j)
		{
			if (src[j] > src[j + 1])
			{
				int temp = src[j];
				src[j] = src[j + 1];
				src[j + 1] = temp;
				flag = false;//标志为假 还需要排序
			}
		}
		
		if (flag)
		{
			cout << "已然有序 不需要继续排序" << endl;
			break;//
		}
		cout << "第" << setw(2) << i + 1 << "次排序后，结果为：";
		for (int k = 0; k <= size - 1; ++k)
		{
			cout << setw(2) << src[k] << " ";
		}
		cout << endl;
	}
}
int main1()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };
	vector<int>myvec(begin(Array), end(Array));

	Bubble_sort1(myvec);
	cout << "排序结束后，最终结果：";
	for (int val : myvec)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	cout << "*******************************************" << endl;
	vector<int>myvec2(begin(Array), end(Array));
	Bubble_sort2(myvec2);

	cout << "排序结束后，最终结果：";
	for (int val : myvec2)
	{
		cout << setw(2) << val << " ";
	}
	cout << endl;
	return 0;
}

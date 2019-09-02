
/**══════════════════════════════════╗
*作    者：songjinzhou                                                 ║
*CSND地址：https://blog.csdn.net/weixin_43949535                       ║
***GitHub：https://github.com/TsinghuaLucky912/My_own_C-_study_and_blog║
*═══════════════════════════════════╣
*创建时间：2019年9月2日22:35:47
*功能描述：
*
*
*═══════════════════════════════════╣
*结束时间: 2019年9月3日00:00:52
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

const int Bucket_Num = 10;//这里当然是10个桶

/*
123
123 / 1 % 10 == 3
123 / 10 % 10 == 2
123 / 100 % 10 == 1
*/
//得到这个thisbit位 上面的数字
int Get_thisBit_num(int data, int thisbit)
{
	return data / thisbit % 10;
}

//递归实现版本
void Radix_Sort_MSD_Operator(vector<int>& this_bucket, vector<int>& temp_src, int this_bit)
{
	if (this_bit == 0)
		return;
	//先把桶空间给 开辟出来
	vector<vector<int>>myBucket;
	myBucket.resize(Bucket_Num);//10个桶

	//接下来就是 把*it  或者说 this_bucket里面的数据，从最后一个读入，MSD 最高位放入桶中
	for (vector<int>::iterator it = this_bucket.end() - 1; it >= this_bucket.begin(); --it)
	{
		if (it == this_bucket.begin())
		{
			int num = Get_thisBit_num(*it, this_bit);
			myBucket[num].push_back(*it);//放入桶里面
			break;
		}
		int num = Get_thisBit_num(*it, this_bit);
		myBucket[num].push_back(*it);//放入桶里面
	}

	for (vector<vector<int>>::iterator it = myBucket.begin(); it != myBucket.end(); ++it)
	{
		if ((*it).size() == 1)
		{
			for (int val : *it)
			{
				temp_src.push_back(val);
			}
		}
		else if ((*it).size() > 1)
		{
			Radix_Sort_MSD_Operator(*it, temp_src, this_bit / 10);
			for (int val : *it)
			{
				temp_src.push_back(val);
			}
		}
	}
}

void Radix_Sort_MSD(vector<int>& src)
{
	int size = src.size();

	//先把桶空间给 开辟出来
	vector<vector<int>>myBucket;
	myBucket.resize(Bucket_Num);//10个桶

	//得到这个数列里面最大的数据的位数 是几位
	int maxval = src[0];
	for (int i = 1; i < size; ++i)
	{
		if (src[i] > maxval)
			maxval = src[i];
	}
	int Bit_of_maxval = 0;
	while (maxval != 0)
	{
		Bit_of_maxval++;
		maxval /= 10;
	}
	//得到了 位数就可以知道 要进行几次的递归排序操作了

	//需要一个临时的temp_src来存放结果
	vector<int>temp_src;
	temp_src.reserve(size);

	int this_bit = pow(10, Bit_of_maxval - 1);//3位数 就是100

	//首先是 把src数据，从最后一个读入，MSD 最高位放入桶中
	for (vector<int>::iterator it=src.end()-1;it>=src.begin();--it)
	{
		if (it == src.begin())
		{
			int num = Get_thisBit_num(*it, this_bit);
			myBucket[num].push_back(*it);//放入桶里面
			break;
		}
		int num = Get_thisBit_num(*it, this_bit);
		myBucket[num].push_back(*it);//放入桶里面
	}

	for (vector<vector<int>>::iterator it = myBucket.begin(); it != myBucket.end(); ++it)
	{
		if ((*it).size() == 1)
		{
			for (int val : *it)
			{
				temp_src.push_back(val);
			}
		}
		else if((*it).size() > 1)
		{
			Radix_Sort_MSD_Operator(*it, temp_src, this_bit / 10);
		}
	}

	//排序结束了，temp_src数据得给src了
	src.clear();
	for (int val : temp_src)
	{
		src.push_back(val);
	}
}

int main()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };
	int Array2[] = { 9,21,32,13,34,45,26,37,8,10,26 };
	int Array3[] = { 123,234,564,765,876,324,651,874,432,23,5,0 };
	int Array4[] = { 19,19,21,34,16,78 };
	vector<int>myvec(begin(Array), end(Array));

	cout << "数列初始状态：";
	for (int val : myvec)
		cout << setw(2) << val << " ";
	cout << endl;
	cout << "/*------------------------------------*/" << endl;
	Radix_Sort_MSD(myvec);
	cout << "/*------------------------------------*/" << endl;
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

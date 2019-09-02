
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
#include <iomanip>
#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

const int BucketNum = 6;

void Bucket_sort(vector<int>& src)
{
	int size = src.size();
	int minval = src[0];
	int maxval = src[0];

	for (int i = 1; i < size; ++i)
	{
		if (src[i] > maxval)
			maxval = src[i];
		if (src[i] < minval)
			minval = src[i];
	}
	int bucket_range = ceil((maxval - minval + 1) / BucketNum) + 1;//每个桶的范围  这里大概是9

	//所以说：从0号到5号桶的范围：【1，10）、【10，19）、【19，28）、【28，37）、【37，46）、【46，55）
	vector<vector<int>>myvec;
	myvec.resize(BucketNum);//先整出来 6个桶

	//函数上的映射关系：floor((data – min) / Arr_val),就可以实现具体元素的按位就座了
	int index_of_data = 0;//每个数据的桶的下标
	for (int val : src)
	{
		index_of_data = floor((val - minval) / bucket_range);
		myvec[index_of_data].push_back(val);
	}
	//到此为止每个数据，都进入了 属于自己的桶，接下来进行排序
	for (int i = 0; i < BucketNum; ++i)
	{
		sort(myvec[i].begin(), myvec[i].end());//桶内排序
	}
	src.clear();

	for (int i = 0; i < BucketNum; ++i)
	{
		vector<int>::iterator it = myvec[i].begin();
		while (it != myvec[i].end())
		{
			src.push_back(*it);
			it++;
		}
	}
}
int main()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };
	vector<int>myvec(begin(Array), end(Array));
	
	cout << "数列初始状态：";
	for (int val : myvec)
		cout << setw(2) << val << " ";
	cout << endl;
	cout << "/*------------------------------------*/" << endl;
	Bucket_sort(myvec);
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

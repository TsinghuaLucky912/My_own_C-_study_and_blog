
/**══════════════════════════════════╗
*作    者：songjinzhou                                                 ║
*CSND地址：https://blog.csdn.net/weixin_43949535                       ║
***GitHub：https://github.com/TsinghuaLucky912/My_own_C-_study_and_blog║
*═══════════════════════════════════╣
*创建时间：2019年9月2日15:39:58                                                            
*功能描述：                                                            
*                                                                      
*                                                                      
*═══════════════════════════════════╣
*结束时间: 2019年9月2日16:31:39                                                            
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
int Get_thisBit_num(int data,int thisbit)
{
	return data / thisbit % 10;
}

void Radix_Sort_LSD(vector<int>& src)
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
	//得到了 位数就可以知道 要进行几次的排序操作了

	//需要一个临时的temp_src来存放中间结果
	vector<int>temp_src;
	temp_src.reserve(size);

	//接下来进行每一位的操作
	//因为是LSD操作，从低位到高位
	int thisbit = 1;//从个位开始
	for (int i = 0; i < Bit_of_maxval ; ++i)//控制次数
	{
		if (i == 0)
		{
			for (int val : src)
			{
				int num = Get_thisBit_num(val, thisbit);
				myBucket[num].push_back(val);//放入桶里面
			}
		}
		else//此后读的就是临时数组里面的数据元素了
		{
			for (int val : temp_src)
			{
				int num = Get_thisBit_num(val, thisbit);
				myBucket[num].push_back(val);//放入桶里面
			}

			temp_src.clear();//读完了之后 temp_src数组也得清空
		}
		
		//每个数字 放入桶里面之后，接下来就需要把每个桶里面的数据依次放入到temp_src里面了
		//但是temp_src里面的 仍旧是无序的中间结果而已
		for (int i = 0; i < Bucket_Num; ++i)
		{
			vector<int>::iterator it = myBucket[i].begin();
			while (it != myBucket[i].end())
			{
				temp_src.push_back(*it);
				it++;
			}
			myBucket[i].clear();//该号桶 下次还要用呢
		}

		thisbit *= 10;//到下一位了
	}
	src.clear();

	//排序结束了，temp_src数据得给src了
	for (int val : temp_src)
	{
		src.push_back(val);
	}
}

int main()
{
	int Array[] = { 1,50,38,5,47,15,36,26,27,2,46,4,19,44,48 };
	int Array2[] = { 9,21,32,13,34,45,26,37,8,10 };
	int Array3[] = { 123,234,564,765,876,324,651,874,432,23,5,0 };
	vector<int>myvec(begin(Array3), end(Array3));

	cout << "数列初始状态：";
	for (int val : myvec)
		cout << setw(2) << val << " ";
	cout << endl;
	cout << "/*------------------------------------*/" << endl;
	Radix_Sort_LSD(myvec);
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

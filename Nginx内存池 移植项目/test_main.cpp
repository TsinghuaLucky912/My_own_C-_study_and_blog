#include "Nginx_memeory_pool.h"
#include <cstdio>

typedef struct Data stData;
struct Data//这是 我们申请大块内存放的东西  假装它需要512字节
{
	char* ptr;//关键是这两个指针代表的资源类型也还是不一样的
	FILE* pfile;//指向外部资源不同 处理关闭函数也就不同
};
//下面两个是 用户自己提供的外部资源清理函数

void func1(void* p1)
{
	char* p = (char*)p1;
	printf("free ptr mem!\n");
	free(p);
}
void func2(void * pf1)
{
	FILE* pf = (FILE*)pf1;
	printf("close file!\n");
	fclose(pf);
}
int main()
{
	//memorypool.ngx_create_pool  可以实现在 Nginx_memory_pool的构造函数当中
	Nginx_memory_pool memorypool;
	// 512 - sizeof(ngx_pool_t) - 4095   =>   较小值放入max域
	ngx_pool_s* pool = (ngx_pool_s*)memorypool.ngx_create_pool(512);//创建一个内存池
	if (pool == nullptr)
	{
		printf("ngx_create_pool fail...");
		return -1;
	}

	void* p1 = memorypool.ngx_palloc(128); // 从小块内存池分配的
	if (p1 == nullptr)
	{
		printf("ngx_palloc 128 bytes fail...");
		return -2;
	}

	stData* p2 = (stData*)memorypool.ngx_palloc(512); // 从大块内存池分配的
	if (p2 == nullptr)
	{
		printf("ngx_palloc 512 bytes fail...");
		return -3;
	}
	p2->ptr =(char *) malloc(12);//指向的是堆上的一块资源
	strcpy(p2->ptr, "hello world");
	p2->pfile = fopen("data.txt", "w");//指向的是一个打开的文件


	//这里还可以通过 绑定器（把函数指针保留下来）来实现 函数回调
	//给回调函数 传入相应的参数：释放内存的起始地址
	ngx_pool_cleanup_s* c1 = memorypool.ngx_pool_cleanup_add(sizeof(char*));
	c1->handler = func1;//保存回调函数
	c1->data = p2->ptr;

	ngx_pool_cleanup_s* c2 = memorypool.ngx_pool_cleanup_add(sizeof(FILE*));
	c2->handler = func2;
	c2->data = p2->pfile;//作为参数 传给回调函数

	memorypool.ngx_destroy_pool();
	// 1.调用所有的预置的清理函数 2.释放大块内存 3.释放小块内存池所有内存
	//memorypool.ngx_destroy_pool  可以实现在 Nginx_memory_pool的析造函数当中
	return 0;
}
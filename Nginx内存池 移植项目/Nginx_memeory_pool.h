#pragma once
/*
项目移植1：Nginx内存池源码移植，并用OOP思想封装实现项目
*/
#include <cstdlib>
#include <string>
using u_char=unsigned char;      //类型重定义
using ngx_uint_t=unsigned int;
struct ngx_pool_s;               //类型前置声明一下：
/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
const int ngx_pagesize = 4096;//默认一个物理页面的大小 4K

//Nginx小块内存池可以分配的最大空间
const int NGX_MAX_ALLOC_FROM_POOL = ngx_pagesize - 1;

const int NGX_DEFAULT_POOL_SIZE = 16 * 1024;//16K   Nginx内存池默认开辟大小

//内存池大小按照16字节进行对齐
const int NGX_POOL_ALIGNMENT = 16;

//类似于SGI STL二级空间配置器内存池的 _S_round_up函数
#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))//上调函数调用,调整到16字节的倍数。
//小块内存 分配考虑字节对齐时的单位。32位4字节  64位8字节
#define NGX_ALIGNMENT   sizeof(unsigned long)    /* platform word */

//把指针p 调整到a的倍数
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

//把buffer缓冲区清0
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)

//清理回调函数的类型  定义了一个函数指针
typedef void (*ngx_pool_cleanup_pt)(void* data);

//清除大块内存之前，清除外部资源的  清理回调操作
struct ngx_pool_cleanup_s {
	ngx_pool_cleanup_pt   handler;//定义了一个 函数指针，保存清理操作的回调函数
	void* data;                   //传给回调函数的参数
	ngx_pool_cleanup_s* next;     //下一个 cleanup清理操作（handler）
};


//大块内存的头部信息
struct ngx_pool_large_s {
	ngx_pool_large_s* next;      //所有的大块内存分配都是被串在一条链表上
	void* alloc;                 //保存分配出去的大块内存的起始地址
};


//分配小块内存的 内存池头部数据信息
struct ngx_pool_data_t {         //  数据头
	u_char* last;                //小块内存池  可分配的空闲内存的起始地址
	u_char* end;                 //小块内存池  可分配的空闲内存的末尾地址
	ngx_pool_s* next;            //把 多次开辟的小块内存的 内存池连接起来
	ngx_uint_t            failed;//每个小块内存 内存池分配失败，则失败次数++
} ;

//Nginx内存池 内存头信息和管理信息
struct ngx_pool_s {              
	ngx_pool_data_t       d;     //当前小块内存池的使用情况
	size_t                max;   //小块内存分配和大块内存分配的分界线
	ngx_pool_s* current;         //指向第一个提供小块内存分配的小块内存池
	ngx_pool_large_s* large;     //指向大块内存的链表 入口地址
	ngx_pool_cleanup_s* cleanup; //指向所有预置的清理操作 回调函数的链表入口地址
};

//Nginx小块内存池最小的size容量 调整为NGX_POOL_ALIGNMENT的临近倍数
//最起码这个内存池得把这个 内存头和2个ngx_pool_large_s内存头
const int NGX_MIN_POOL_SIZE = ngx_align((sizeof(ngx_pool_s) +
	2 * sizeof(ngx_pool_large_s)), NGX_POOL_ALIGNMENT);
class Nginx_memory_pool          //Nginx内存池类
{
public:
	//内存池创建函数 size大小的内存池（不能超过1个页面大小）
	void* ngx_create_pool(size_t size);

	//考虑内存字节对齐，向内存池申请size字节内存
	void* ngx_palloc(size_t size);
	//不考虑内存字节对齐，向内存池申请size字节内存
	void* ngx_pnalloc(size_t size);
	//调用ngx_palloc函数开辟内存，但是会初始化内存为 0
	void* ngx_pcalloc(size_t size);

	//释放大块内存
	void  ngx_pfree(void* p);

	//内存重置函数
	void ngx_reset_pool();

	//内存池的销毁函数
	void ngx_destroy_pool();
	
	//添加用户提供的  清理回调函数的函数
	ngx_pool_cleanup_s* ngx_pool_cleanup_add(size_t size);
private:
	ngx_pool_s* pool_;          //指向Nginx内存池的入口指针

	//小块内存分配
	void* ngx_palloc_small(size_t size, ngx_uint_t align)
	{
		u_char* m;
		ngx_pool_s* p;

		p = pool_->current;

		do {
			m = p->d.last;

			if (align) {
				m = ngx_align_ptr(m, NGX_ALIGNMENT);
			}

			if ((size_t)(p->d.end - m) >= size) {
				p->d.last = m + size;

				return m;
			}

			p = p->d.next;

		} while (p);

		return ngx_palloc_block(size);
	}

	//分配新的内存池
	void* ngx_palloc_block(size_t size)
	{
		u_char* m;
		size_t       psize;
		ngx_pool_s* p, * newpool;

		psize = (size_t)(pool_->d.end - (u_char*)pool_);

		m = (u_char*)malloc(psize);
		if (m == nullptr) {
			return nullptr;
		}

		newpool = (ngx_pool_s*)m;

		newpool->d.end = m + psize;
		newpool->d.next = nullptr;
		newpool->d.failed = 0;

		m += sizeof(ngx_pool_data_t);
		m = ngx_align_ptr(m, NGX_ALIGNMENT);
		newpool->d.last = m + size;

		for (p = pool_->current; p->d.next; p = p->d.next) {
			if (p->d.failed++ > 4) {
				pool_->current = p->d.next;
			}
		}

		p->d.next = newpool;

		return m;
	}

	//大块内存分配
	void* ngx_palloc_large(size_t size)
	{
		void* p;
		ngx_uint_t         n;
		ngx_pool_large_s* large;

		p = malloc(size);
		if (p == nullptr) {
			return nullptr;
		}

		n = 0;

		for (large = pool_->large; large; large = large->next) {
			if (large->alloc == nullptr) {
				large->alloc = p;
				return p;
			}

			if (n++ > 3) {
				break;
			}
		}

		large = (ngx_pool_large_s*)ngx_palloc_small(sizeof(ngx_pool_large_s), 1);
		if (large == nullptr) {
			free(p);
			return nullptr;
		}

		large->alloc = p;
		large->next = pool_->large;
		pool_->large = large;

		return p;
	}
};




















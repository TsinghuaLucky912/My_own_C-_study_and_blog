#include "Nginx_memeory_pool.h"


//主要是 初始化和内存分配 清理有关的管理信息
//内存池创建函数 size大小的内存池（不能超过1个页面大小）
void * Nginx_memory_pool::ngx_create_pool(size_t size)
{
	ngx_pool_s* p;

	p = (ngx_pool_s*)malloc(size);
	if (p == nullptr) {
		return nullptr;
	}

	p->d.last = (u_char*)p + sizeof(ngx_pool_s);
	p->d.end = (u_char*)p + size;
	p->d.next = nullptr;
	p->d.failed = 0;

	size = size - sizeof(ngx_pool_s);//减去内存头部分
	p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

	p->current = p;
	p->large = nullptr;
	p->cleanup = nullptr;

	pool_ = p;//这个地方 p是一个局部变量
	return p;
}
/*
size = size - sizeof(ngx_pool_s);//减去内存头部分
完了之后，size要和NGX_MAX_ALLOC_FROM_POOL比较。
max的值：小则size，大则最多NGX_MAX_ALLOC_FROM_POOL 不能超过一个页面
因为内存分配是通过last指针偏移完成的，不在一个物理页面上：内存不一定连续。
这样的话：再通过指针偏移，效率会很低的。

*/
//考虑内存字节对齐，向内存池申请size字节内存
void* Nginx_memory_pool::ngx_palloc(size_t size)
{
	if (size <= pool_->max) {
		return ngx_palloc_small(size, 1);
	}
	return ngx_palloc_large(size);
}

//不考虑内存字节对齐，向内存池申请size字节内存
void* Nginx_memory_pool::ngx_pnalloc(size_t size)
{
	if (size <= pool_->max) {
		return ngx_palloc_small(size, 0);
	}
	return ngx_palloc_large(size);
}

//调用ngx_palloc函数开辟内存，但是会初始化内存为 0
void* Nginx_memory_pool::ngx_pcalloc(size_t size)
{
	void* p;

	p = ngx_palloc(size);
	if (p) {
		ngx_memzero(p, size);//清0
	}

	return p;
}

//释放大块内存
void Nginx_memory_pool::ngx_pfree(void* p)
{
	ngx_pool_large_s* l;

	for (l = pool_->large; l; l = l->next) {
		if (p == l->alloc) {
			free(l->alloc);
			l->alloc = nullptr;

			return;
		}
	}
}

//内存重置函数
void Nginx_memory_pool::ngx_reset_pool()
{
	ngx_pool_s* p;
	ngx_pool_large_s* l;

	for (l = pool_->large; l; l = l->next) {
		if (l->alloc) {
			free(l->alloc);
		}
	}
	/*
	for (p = pool_; p; p = p->d.next) {
		p->d.last = (u_char*)p + sizeof(ngx_pool_s);
		p->d.failed = 0;
	}
	*/
	//处理第一个内存块 内存池
	p = pool_;
	p->d.last = (u_char*)p + sizeof(ngx_pool_s);
	p->d.failed = 0;
	//处理后面的内存块 内存池
	for (p = p->d.next; p; p = p->d.next) {
		p->d.last = (u_char*)p + sizeof(ngx_pool_s);
		p->d.failed = 0;
	}
	pool_->current = pool_;
	pool_->large = nullptr;
}

//内存池的销毁函数
void Nginx_memory_pool::ngx_destroy_pool()
{
	ngx_pool_s* p, * n;
	ngx_pool_large_s* l;
	ngx_pool_cleanup_s* c;

	for (c = pool_->cleanup; c; c = c->next) {
		if (c->handler) {
			c->handler(c->data);
		}
	}

	for (l = pool_->large; l; l = l->next) {
		if (l->alloc) {
			free(l->alloc);
		}
	}

	for (p = pool_, n = pool_->d.next; /* void */; p = n, n = n->d.next) {
		free(p);

		if (n == nullptr) {
			break;
		}
	}
}

//添加用户提供的  清理回调函数的函数
ngx_pool_cleanup_s* Nginx_memory_pool::ngx_pool_cleanup_add(size_t size)
{
	ngx_pool_cleanup_s* c;

	c = (ngx_pool_cleanup_s*)ngx_palloc(sizeof(ngx_pool_cleanup_s));
	if (c == nullptr) {
		return nullptr;
		}

	if (size) {
		c->data = ngx_palloc(size);
		if (c->data == nullptr) {
			return nullptr;
		}

	}
	else {
		c->data = nullptr;
	}

	c->handler = nullptr;
	c->next = pool_->cleanup;

	pool_->cleanup = c;

	
	return c;
}

#pragma once
#include <mutex>
#include <iostream>
/*
这个虽然也是对malloc 和 free的简单封装

但是却提供了oom的释放内存的回调函数：当内存开辟失败，可以调用用户预置的回调函数。
没有则抛异常
*/
template <int __inst>
class __malloc_alloc_template
{
private:
	static void* _S_oom_malloc(size_t);
	static void* _S_oom_realloc(void*, size_t);
	static void (*__malloc_alloc_oom_handler)();
public:
	static void* allocate(size_t __n)
	{
		void* __result = malloc(__n);
		if (0 == __result) __result = _S_oom_malloc(__n);
		return __result;
	}

	static void deallocate(void* __p, size_t /* __n */)
	{
		free(__p);
	}

	static void* reallocate(void* __p, size_t /* old_sz */, size_t __new_sz)
	{
		void* __result = realloc(__p, __new_sz);
		if (0 == __result) __result = _S_oom_realloc(__p, __new_sz);
		return __result;
	}

	static void (*__set_malloc_handler(void (*__f)()))()
	{
		void (*__old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = __f;
		return(__old);
	}
};

//静态成员 函数指针
template <int __inst>
void (*__malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = nullptr;

template <int __inst>
void*__malloc_alloc_template<__inst>::_S_oom_malloc(size_t __n)
{
	void (*__my_malloc_handler)();
	void* __result;

	for (;;) {
		__my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == __my_malloc_handler) { throw std::bad_alloc(); }
		(*__my_malloc_handler)();
		__result = malloc(__n);
		if (__result) return(__result);
	}
}

template <int __inst>
void* __malloc_alloc_template<__inst>::_S_oom_realloc(void* __p, size_t __n)
{
	void (*__my_malloc_handler)();
	void* __result;

	for (;;) {
		__my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == __my_malloc_handler) { throw std::bad_alloc(); }
		(*__my_malloc_handler)();
		__result = realloc(__p, __n);
		if (__result) return(__result);
	}
}
typedef __malloc_alloc_template<0> malloc_alloc;
//////////////////////////////////////////////////////////////////////////////////

template<typename T>
class myallocator
{
public:
	//在vector容器的代码上 用到了这个空间配置器的value_type类型
	using value_type = T;

	//需要自己去添加一些 构造函数
	constexpr myallocator() noexcept { // construct default allocator (do nothing)
	}

	constexpr myallocator(const myallocator&) noexcept = default;

	//这相当于一个构造函数的模板，
	//可以用任意类型实例化的myallocator来给当前T类型 实例化的myallocator进行拷贝构造
	//但事实上 里面什么也没有做
	template <class _Other>
	constexpr myallocator(const myallocator<_Other>&) noexcept { 
		//construct from a related allocator (do nothing)
	}

	//开辟内存   /* __n must be > 0      */
	T* allocate(size_t __n)
	{
		/*
		C++ STL容器里面的空间配置器在容器使用的时候
		调用空间配置器的allocate方法的时候：按字节数（内存）传入
		但是SGI 的这个传入的__n 是元素的个数
		所以这里做如下 调整
		*/
		__n *= sizeof(T);//这才是 元素需要的内存大小
		void* __ret = 0;

		if (__n > (size_t)_MAX_BYTES) {
			__ret = malloc_alloc::allocate(__n);
		}
		else {
			_Obj* volatile* __my_free_list
				= _S_free_list + _S_freelist_index(__n);
			
			//构造函数之中 加锁；出作用域解锁
			std::lock_guard<std::mutex>guard(mtx);

			//因为共用的是同一个内存池
			_Obj*  __result = *__my_free_list;
			if (__result == 0)
				__ret = _S_refill(_S_round_up(__n));
			else {
				*__my_free_list = __result->_M_free_list_link;
				__ret = __result;
			}
		}

		return (T*)__ret;
	}

	//释放内存   /* __p may not be 0 */
	//把chunk块 归还到内存池里面。归还的时候 也是要加锁的
	void deallocate(void* __p, size_t __n)//这里传入的__n就是实际字节数
	{
		if (__n > (size_t)_MAX_BYTES)
		{
			malloc_alloc::deallocate(__p, __n);
		}
		else {
			_Obj* volatile* __my_free_list
				= _S_free_list + _S_freelist_index(__n);
			_Obj* __q = (_Obj*)__p;

			//构造函数之中 加锁；出作用域解锁
			std::lock_guard<std::mutex>guard(mtx);

			__q->_M_free_list_link = *__my_free_list;
			*__my_free_list = __q;
			// lock is released here
		}
	}

	//内存 扩容/缩容 函数
	void* reallocate(void* __p, size_t __old_sz, size_t __new_sz)//新旧空间大小
	{
		void* __result;
		size_t __copy_sz;

		if (__old_sz > (size_t)_MAX_BYTES && __new_sz > (size_t)_MAX_BYTES) {
			return(realloc(__p, __new_sz));
		}
		if (_S_round_up(__old_sz) == _S_round_up(__new_sz)) return(__p);
		__result = allocate(__new_sz);
		__copy_sz = __new_sz > __old_sz ? __old_sz : __new_sz;
		memcpy(__result, __p, __copy_sz);
		deallocate(__p, __old_sz);
		return(__result);
	}

	//对象构造
	void construct(T* __p, const T& val)
	{
		new(__p)T(val);
	}

	//对象析构
	void destroy(T* __p)
	{ 
		__p->~T(); 
	}
private:
	//辅助函数都写成私有的
	enum { _ALIGN = 8 };//16个自由链表 从8---128，以8字节为对齐方式
	enum { _MAX_BYTES = 128 };//内存池最大的chunk块
	enum { _NFREELISTS = 16 }; // _MAX_BYTES/_ALIGN 自由链表的个数（静态链表）

	//每一个chunk块的头信息
	union _Obj
	{
		union _Obj* _M_free_list_link;//存储下一个chunk块的地址
		char _M_client_data[1];    /* The client sees this.        */
	};

	//因为在多线程下使用，防止线程缓存
	//_S_free_list表示 存储自由链表 的起始地址
	static _Obj* volatile _S_free_list[_NFREELISTS];

	//定义一把静态的锁.内存池基于freelist实现的  需要考虑线程安全操作
	static std::mutex mtx;

	//将__bytes 调整到最邻近的8字节的倍数
	static size_t _S_round_up(size_t __bytes)
	{
		return (((__bytes)+(size_t)_ALIGN - 1) & ~((size_t)_ALIGN - 1));
	}

	//返回__bytes大小的小内存块位于freelist 的元素位下标
	static size_t _S_freelist_index(size_t __bytes)
	{
		return (((__bytes)+(size_t)_ALIGN - 1) / (size_t)_ALIGN - 1);
	}

	//当前内存池已分配的内存chunk块 的使用情况
	// Chunk allocation state.
	static char* _S_start_free;
	static char* _S_end_free;
	static size_t _S_heap_size;

	//负责分配自由链表上的 chunk块的
	static char*_S_chunk_alloc(size_t __size,int& __nobjs)
	{
		char* __result;
		size_t __total_bytes = __size * __nobjs;
		size_t __bytes_left = _S_end_free - _S_start_free;

		if (__bytes_left >= __total_bytes) {
			__result = _S_start_free;
			_S_start_free += __total_bytes;
			return(__result);
		}
		else if (__bytes_left >= __size) {
			__nobjs = (int)(__bytes_left / __size);
			__total_bytes = __size * __nobjs;
			__result = _S_start_free;
			_S_start_free += __total_bytes;
			return(__result);
		}
		else {
			size_t __bytes_to_get =
				2 * __total_bytes + _S_round_up(_S_heap_size >> 4);
			// Try to make use of the left-over piece.
			if (__bytes_left > 0) {
				_Obj* volatile* __my_free_list =
					_S_free_list + _S_freelist_index(__bytes_left);

				((_Obj*)_S_start_free)->_M_free_list_link = *__my_free_list;
				*__my_free_list = (_Obj*)_S_start_free;
			}
			_S_start_free = (char*)malloc(__bytes_to_get);
			if (nullptr == _S_start_free) {
				size_t __i;
				_Obj* volatile* __my_free_list;
				_Obj* __p;
				// Try to make do with what we have.  That can't
				// hurt.  We do not try smaller requests, since that tends
				// to result in disaster on multi-process machines.
				for (__i = __size;
					__i <= (size_t)_MAX_BYTES;
					__i += (size_t)_ALIGN) {
					__my_free_list = _S_free_list + _S_freelist_index(__i);
					__p = *__my_free_list;
					if (0 != __p) {
						*__my_free_list = __p->_M_free_list_link;
						_S_start_free = (char*)__p;
						_S_end_free = _S_start_free + __i;
						return(_S_chunk_alloc(__size, __nobjs));
						// Any leftover piece will eventually make it to the
						// right free list.
					}
				}
				_S_end_free = 0;	// In case of exception.
				_S_start_free = (char*)malloc_alloc::allocate(__bytes_to_get);
				// This should either throw an
				// exception or remedy the situation.  Thus we assume it
				// succeeded.
			}
			_S_heap_size += __bytes_to_get;
			_S_end_free = _S_start_free + __bytes_to_get;
			return(_S_chunk_alloc(__size, __nobjs));
		}
	}

	//把分配好的chunk块进行一个连接的
	static void*_S_refill(size_t __n)
	{
		int __nobjs = 20;
		char* __chunk = _S_chunk_alloc(__n, __nobjs);
		_Obj* volatile* __my_free_list;
		_Obj* __result;
		_Obj* __current_obj;
		_Obj* __next_obj;
		int __i;

		if (1 == __nobjs) return(__chunk);
		__my_free_list = _S_free_list + _S_freelist_index(__n);

		/* Build free list in chunk */
		__result = (_Obj*)__chunk;
		*__my_free_list = __next_obj = (_Obj*)(__chunk + __n);
		for (__i = 1; ; __i++) {
			__current_obj = __next_obj;
			__next_obj = (_Obj*)((char*)__next_obj + __n);
			if (__nobjs - 1 == __i) {
				__current_obj->_M_free_list_link = 0;
				break;
			}
			else {
				__current_obj->_M_free_list_link = __next_obj;
			}
		}
		return(__result);
	}
};

//静态成员变量需要在类外进行初始化
template <typename T>
char* myallocator<T>::_S_start_free = nullptr;

template <typename T>
char* myallocator<T>::_S_end_free = nullptr;

template <typename T>
size_t myallocator<T>::_S_heap_size = 0;

template <typename T>
typename myallocator<T>::_Obj* volatile myallocator<T>::_S_free_list[_NFREELISTS] =
{//typename 告诉编译器这是一个 类型定义
	nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr,
	nullptr,nullptr,nullptr,nullptr
};

//在类外 初始化这把锁
template <typename T>
std::mutex myallocator<T>::mtx;
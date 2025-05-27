#pragma once
#include <stdio.h>
#include <memory>
#include <immintrin.h>
#include <any>
#include <string>

#define ALIGN_VALUE 16
#define STACK_SIZE 10000
#define STACK_ELEMENT_COUNT 300

#define ON_CONDITION(cond, action) { if(cond) action }
#define ERROR_EXIT(msg) { printf(msg); exit(-1); }
#define CRITICAL 2
#define WARNING 1
#define OK 0

#ifdef _DEBUG
#define ON_DEBUG_CHECK(x) x
#else
#define ON_DEBUG_CHECK(x) do {} while 0
#endif


typedef unsigned int uint;

/*
	THIS NEEDS TO BE REIMPLEMENTED!!!!
	THIS NEEDS TO BE REIMPLEMENTED!!!!
*/
template<typename T>
class unique_ptr
{
	union 
	{
		void* p_address;
		T* obj;
	};
	unqiue_ptr() = delete;
	unique_ptr(unique_ptr<T>& transfer);

	template<typename ...Args>
	unique_ptr(Args&& ... T_args) : obj(&T {T_args ... })
	{
		*obj = T { T_args ... };
	}
};



template<typename T>
struct node
{
	T val;
	node* next;
};




template<typename T>
class Buffer
{
private:
	union 
	{
		T* objects;
		void* mem_address;
	};
	size_t m_size;

public:
	Buffer(const Buffer&) = delete;
	Buffer(Buffer&&) = delete;
	Buffer& operator=(const Buffer&) = delete;
	Buffer& operator=(Buffer&&) = delete;
	Buffer() {} // Only use init as default constructor

	template<uint Size> void init()
	{
		memset(&mem_address, 0, sizeof(T) * Size);
	}
	Buffer(std::initializer_list<T>& initializer): m_size(initializer.size())
	{
		mem_address = _aligned_malloc(sizeof(T) * m_size, ALIGN_VALUE);
		memcpy(&mem_address, &initializer, sizeof(T) * m_size);
	}

	~Buffer()
	{
		_aligned_free(&mem_address);
	}

	T& operator[](uint&& index)
	{
		ON_DEBUG_CHECK(
			ON_CONDITION
			(
				index > m_size,
				ERROR_EXIT(("Invalid Memory Access In Buffer Object of size %d. Index given was %d", m_size, (unsigned int)index))
			)
		);
		return objects[index];
	}

	const T& operator[](uint&& index) const
	{
		ON_DEBUG_CHECK(
			ON_CONDITION
			(
				index > m_size,
				ERROR_EXIT(("Invalid Memory Access In Buffer Object of size %d. Index given was %d", m_size, index))
			)
		);
		return objects[index];
	}

	T* begin() { return &mem_address[0]; }
	T* end() { return &mem_address[sizeof(T) * m_size - 1]; }
};




struct ObjectInfo
{
	std::string T_name;
	uint size;
	void* address;
};

template<uint Size>
class _buffer
{
private:
	void* buf;
public:
	_buffer(const _buffer&) = delete;
	_buffer(_buffer&&) = delete;
	_buffer& operator=(const _buffer&) = delete;
	_buffer& operator=(_buffer&&) = delete;

	_buffer()
	{
		buf = _aligned_malloc(Size, ALIGN_VALUE);
	}
	~_buffer()
	{
		_aligned_free(&buf);
	}

	void* begin() { return buf; }
	void* end()   { return buf; }

};

class Stack_any
{
private:
	_buffer<STACK_SIZE> stk;
	void* top_address;
	uint increment;
public:
	Buffer<ObjectInfo> booker;
	
	Stack_any() : stk(), top_address(stk.begin()), increment(0) 
	{
		booker.init<STACK_ELEMENT_COUNT>();
	}

	template<typename T, typename ... Args>
	void push(Args&& ... T_args)
	{
		
		top_address += sizeof(T);

		booker[increment] = {};
	}

	std::any pop()
	{

	}
};

class Tracker
{
	Stack_any log;

};

struct MemAllocator
{

};

struct StackAlloc : public MemAllocator
{

};

struct TableAlloc : public MemAllocator
{

};

struct BuddyAlloc : public MemAllocator
{

};
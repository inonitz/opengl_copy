#pragma once
#pragma warning(disable: 28196 28251 6386)
#include <assert.h>
#include <iostream>
#include "Array.h"

#ifdef _DEBUG
#define D(x) (x)
#else
#define D(x) do {} while(0)
#endif

#define ERR_EXIT(str)				    std::cout << str; exit(-1);
#define ON_CONDITION_DO(cond, action)   if(cond) { action } 
#define EXIT_ON_CONDITION(cond, msg)  { if(cond) ERR_EXIT(msg)      }
#define ALIGNMENT_BYTE_COUNT 32


//void* operator new(size_t bytes)
//{
//	void* out = _aligned_malloc(bytes, ALIGNMENT_BYTE_COUNT);
//	ERR_EXIT_CONDITION(out == 0, _aligned_free(out); ERR_EXIT("BAD ALLOCATION"));
//	return out;
//}
//void operator delete(void* p, size_t bytes)
//{
//	_aligned_free(p);
//}

// TODO implement when needed.
class ResourceManager {
private:
public:
};

template<typename T> struct underlying_type      { typedef T t_type; };
template<typename T> struct underlying_type<T&>  { typedef T t_type; };
template<typename T> struct underlying_type<T&&> { typedef T t_type; };
template<typename T> typename underlying_type<T>::t_type && move(T&& arg) { return static_cast<typename underlying_type<T>::t_type&&>(arg); }

// chunk == Space for object of size T.
// BufferSize == amount of chunks available in the buffer.
// T == Generic type for generic Object allocation.
// allocateBlock() == allocate N chunks in the buffer, may not be necessarily contiguous. (?*)

template<typename T>
union Block {
	using MemoryBlock = char[sizeof(T)];
public:
	MemoryBlock data;
	Block* next;
};

template<typename T, unsigned int nElem>
class Pool {
private:
	std::unique_ptr<Block<T>[]> m_BlockBuffer;
	Block<T>* m_availableBlock;
	unsigned int m_freeBlocks;

public:
	Pool(const Pool&) = delete;
	Pool(Pool&&) = delete;
	Pool& operator =(const Pool&) = delete;

	Pool()
	{
		EXIT_ON_CONDITION(nElem != 0, "Can't Allocate 0 Objects.");
		m_BlockBuffer = std::make_unique<Block<T>[]>(nElem);
		m_availableBlock = &m_BlockBuffer[0];

		for (unsigned int i = 1; i < nElem; i++) 
		{
			m_BlockBuffer.get()[i - 1].next = &m_BlockBuffer.get()[i];
		}
		m_BlockBuffer.get()[nElem - 1].next = nullptr;
		m_freeBlocks = nElem;
	}


	T* allocate()
	{
		EXIT_ON_CONDITION(!m_freeBlocks, "Can't Allocate, Object Pool is Full! (" << nElem << ")");
		T* obj_buffer = reinterpret_cast<T*>(m_availableBlock->data);
		m_availableBlock = m_availableBlock->next;
		m_freeBlocks--;
		return obj_buffer;
	}


	template<unsigned int Count> Array<T*> allocateArray()
	{
		EXIT_ON_CONDITION(
			Count < 0 || Count > nElem,
			"Asked for " << Count << "objects, but the Pool is of size " << nElem
		);

		Array<T*> objLocations(Count, true);
		for (unsigned int i = 0; i < Count; i++)
		{
			objLocations.push_back(allocate());
		}
		return objLocations;
	}


	void free_array(Array<T*> arr)
	{
		EXIT_ON_CONDITION(
			m_freeBlocks + arr.size > nElem, 
			"Cannot free more Objects than allocated!"
		);

		m_freeBlocks += arr.size;
		Block<T>* tmp = arr.A[0], first = tmp;

		for (int i = 1; i < arr.size; i++)
		{
			tmp->next = arr.A[i];
			tmp = tmp->next;
		}

		tmp->next = m_availableBlock;
		m_availableBlock = first;
	}


	void free(T* p)
	{
		EXIT_ON_CONDITION((unsigned int)p == 0, "");
		EXIT_ON_CONDITION(
			(unsigned int)p >= (unsigned int)&m_BlockBuffer[0]
			&& 
			(unsigned int)p <= (unsigned int)&m_BlockBuffer[nElem - 1],
			"Pointer must be allocated from the pool"
		);
		m_freeBlocks++;
		Block<T>* next = m_availableBlock;
		m_availableBlock = reinterpret_cast<Block<T>*>(p);
		m_availableBlock->next = next;
	}


	void printAddresses()
	{
		std::cout << "Object Addresses: \n";
		for (unsigned int i = 0; i < nElem; i++)
		{
			std::cout << "Object " << i << ": " << &m_BlockBuffer.get()[i].data << " next Object: " << m_BlockBuffer.get()[i].next << "\n";
		}

		std::cout << "\nAvailable Objects: (" << m_freeBlocks << ")\n";
		Block<T>* tmp = m_availableBlock;
		unsigned int c = 0;
		while (tmp != 0)
		{
			std::cout << "Object " << c << ": " << &tmp->data << " next Object: " << tmp->next << "\n";
			tmp = tmp->next;
			++c;
		}
		std::cout << "     Object address: " << &tmp->data;
	}
};




template<typename T> class shared_ptr
{
private:
	static inline int references = 0;
	T* object;
public:
	shared_ptr() = delete;
	shared_ptr(const shared_ptr<T>&) = delete;
	shared_ptr(shared_ptr<T>& ptr)
	{
		object = ptr.object;
		++references;
	}

	shared_ptr(T&& ref)
	{
		// ref must be dynamically allocated on the heap! 
		++references;
		object = &ref;
		EXIT_ON_CONDITION(!object, "Failed to allocate Object!");
	}

	~shared_ptr() { references--; if (!references) { delete object; object = nullptr; } }

	T              operator *() { return *object; }
	shared_ptr<T>& operator =(const shared_ptr<T>& ptr)
	{
		if (object != ptr.object)
			if (object)
			{
				--references;
				if (!references) delete object;
			}
			else
			{
				object = ptr.object;
				++references;
			}
		return *this;
	}

	std::string debug()
	{
		std::stringstream a("");
		a << "\nobject pointer: " << object;
		a << "\nreference count: " << references;
		return a.str();
	}
	int ref_count() { return references; }
};



// self relative pointer, the objects' location is an offset of the pointer in memory. 
template<typename T>
class r_ptr
{
private:
	uint16_t m_offset;
public:
	T* operator->()
	{
		EXIT_ON_CONDITION(m_offset == 0, "You shouldn't de-reference a NULL pointer");
		return (&m_offset + m_offset);
	}

};

//template<typename T>
//class rel_ptr
//{
//private:
//	uint32_t m_offset;
//
//};




// In Development, don't use yet!
template<typename T>
// Design philosophy - No "moving" objects, just changing pointers.
// Only the pointers are changed, not the data itself.
class Container
{
public:
	Container(Container<T>&& to_move) = delete;
	Container<T>& operator=(Container<T>&& to_move) = delete;


	template<typename ...T_args>
	Container(T_args&& ... arguments)
	{
		// if there is memory manager, preferable to allocate from there [from memory pool for ex]
		T* obj_ptr = new T(arguments...);
		ObjectPointer = obj_ptr;
		PointerReference = ObjectPointer;
	}
	Container(const Container<T>& to_copy)
	{
		T ObjectCopy;
		memcpy(&ObjectCopy, to_copy.ObjectPointer, sizeof(T));
		ObjectPointer = &ObjectCopy;
		PointerReference = ObjectPointer;
	}


	Container<T>& operator=(const Container<T>& to_copy)
	{
		T* ObjectCopy = malloc(sizeof(T));
		memcpy(ObjectCopy, to_copy.ObjectPointer, sizeof(T));
		ObjectPointer = ObjectCopy;
		PointerReference = ObjectPointer;
	}
	Container<T>& operator=(T&& is_moved)
	{
		ObjectPointer = &is_moved;
		return *this;
	}

	T* operator->()
	{
		return ObjectPointer;
	}

private:
	typedef T* pointer;
	typedef T*& pointer_ref;

	pointer ObjectPointer;
	pointer_ref PointerReference;
};





// reference object of type T
template<typename T> class ref
{
private:
	T&& m_obj;
public:
	ref() = delete;
	ref(ref<T>&)  = delete;
	ref(ref<T>&& mve)   = delete;
	ref<T>& operator=(T& obj) = delete; // do not allow multiple references to exist for a T object.

	ref(T&& obj): m_obj(std::move(obj)) {}
	ref<T>& operator=(T&& obj)
	{
		m_obj = std::move(obj);
		return *this;
	}

	template<typename ... TArgs> 
	ref<T>& create(TArgs&& ... args)
	{
		m_obj = T(args ...);
		return *this;
	}

};
#pragma once
#include "Array.h"

template<typename T, typename ... args>
void push_back_array(T arr[], unsigned int& idx, args&&... arg)
{
	arr[idx] = arg;
	++idx;
}

template<typename T>
void push_back_array(T arr[], unsigned int& idx, T&& arg)
{
	arr[idx] = arg;
	++idx;
}

template<typename T>
class init_list
{
private:
	T* m_vec;
public:
	unsigned int
		m_size,
		m_capacity;

	template<typename ... Args>
	init_list(Args&&... args)
	{
		m_size = sizeof...(args);
		m_capacity = 2 * m_size;
		m_vec = new T[m_size];
		for (unsigned int i = 0; i < sizeof...(args); i++)
		{
			push_back_array<T, Args...>(m_vec, i, (T&&)args...);
		}
	}

	T& operator [](unsigned int& idx) { D(assert(idx < m_size)); return m_vec[idx]; }
	T& operator [](unsigned int&& idx) { D(assert(idx < m_size)); return m_vec[idx]; }
	const T& operator [](unsigned int& idx) const { D(assert(idx < m_size)); return m_vec[idx]; }
	const T& operator [](unsigned int&& idx) const { D(assert(idx < m_size)); return m_vec[idx]; }
};

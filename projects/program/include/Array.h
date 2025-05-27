#pragma once
#include <stdexcept>
#include <string>
#include <iostream>

template<typename T>
class Array {
private:
	T* A;
public:
	size_t size;
	size_t cap;
	Array();
	Array(size_t size);
	Array(size_t size, bool behaviour_stack = false);
	Array(size_t size, size_t cap);
	Array(T fill_val, size_t size);
	Array(T fill_val, size_t size, size_t cap);
	Array(const Array<T>&);
	Array(const std::initializer_list<T>&);
	~Array();

	Array<T>& operator =(const Array<T>&);
	Array<T>& operator *=(T&&);
	Array<T>& operator +=(T&&);

	T& operator [](size_t);
	T* get_c();
	void push_back(const T);
	void pop();
	T top();
	T sum();

	size_t size_bytes();
	std::string to_str();

	const T& operator [](size_t) const;
	const T* get_c() const;
};

#include "Array.tpp"

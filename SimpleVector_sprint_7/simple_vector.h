#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include "array_ptr.h"

template <typename Type>
class SimpleVector {
private:
	size_t size = 0;
	size_t capacity = 0;
	ArrayPtr<Type> pointer;


public:
	using Iterator = Type*;
	using ConstIterator = const Type*;

	SimpleVector() noexcept = default;
	explicit SimpleVector(size_t size) : size(size), capacity(size), pointer(size){
		std::fill(begin(), end(), Type());
	}

	SimpleVector(size_t size, const Type& value) : size(size), capacity(size), pointer(size) {
		std::fill(begin(), end(), value);
	}

	SimpleVector(std::initializer_list<Type> init) : size(init.size()), capacity(init.size()), pointer(init.size()) {
		if (size)
			std::copy(init.begin(), init.end(), begin());
	}

	size_t GetSize() const noexcept {
		return size;
	}
	size_t GetCapacity() const noexcept {
		return capacity;
	}
	bool IsEmpty() const noexcept {
		return !size;
	}

	Type& operator[](size_t index) noexcept {
		return pointer[index];
	}
	const Type& operator[](size_t index) const noexcept {
		return pointer[index];
	}
	Type& At(size_t index) {
		if (index >= size)
			throw std::out_of_range("out_of_range");
		return pointer[index];
	}
	const Type& At(size_t index) const {
		if (index >= size)
			throw std::out_of_range("out_of_range");
		return pointer[index];
	}

	// Обнуляет размер массива, не изменяя его вместимость
	void Clear() noexcept {
		size = 0;
	}

	// Изменяет размер массива.
	// При увеличении размера новые элементы получают значение по умолчанию для типа Type
	void Resize(size_t new_size) {
		if (new_size <= size)
			size = new_size;
		else if (new_size <= capacity)
		{
			auto first = end();
			size = new_size;
			std::fill(first, end(), Type());
		}
		else
		{
			ArrayPtr<Type> new_array(new_size);
			std::fill(new_array.Get(), new_array.Get() + new_size, Type());
			std::copy(begin(), end(), new_array.Get());
			pointer.swap(new_array);
			size = new_size;
			capacity = new_size;
		}
	}

	// Возвращает итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	Iterator begin() noexcept {
		return pointer.Get();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	Iterator end() noexcept {
		if (IsEmpty())
			return begin();
		return pointer.Get() + size;
	}

	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator begin() const noexcept {
		return pointer.Get();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator end() const noexcept {
		return pointer.Get() + size;
	}

	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cbegin() const noexcept {
		return pointer.Get();
	}

	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cend() const noexcept {
		return pointer.Get() + size;
	}
};
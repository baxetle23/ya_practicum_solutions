#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include "array_ptr.h"

class ReserveProxyObj {
private:
	size_t capacity_;
public:
	ReserveProxyObj(size_t capacity) : capacity_(capacity){
	}
	size_t GetCapacity(){
		return capacity_;
	}
};

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
	SimpleVector(const SimpleVector& other) : size(other.GetSize()), capacity(other.GetCapacity()), pointer(other.GetSize()) {
		std::copy(other.begin(), other.end(), begin());
	}
	SimpleVector(std::initializer_list<Type> init) : size(init.size()), capacity(init.size()), pointer(init.size()) {
		if (size)
			std::copy(init.begin(), init.end(), begin());
	}
	SimpleVector(ReserveProxyObj obj) {
		Reserve(obj.GetCapacity());
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
		return cbegin();
	}
	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator end() const noexcept {
		return cend();
	}
	// Возвращает константный итератор на начало массива
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cbegin() const noexcept {
		return pointer.Get();
	}
	// Возвращает итератор на элемент, следующий за последним
	// Для пустого массива может быть равен (или не равен) nullptr
	ConstIterator cend() const noexcept {
		if (IsEmpty())
			return cbegin();
		return pointer.Get() + size;
	}

	void Reserve(size_t new_capacity) {
		if (new_capacity > capacity)
		{
			ArrayPtr<Type> new_array(new_capacity);
			std::copy(begin(), end(), new_array.Get());
			pointer.swap(new_array);
			capacity = new_capacity;
		}
	}

	// Добавляет элемент в конец вектора
	// При нехватке места увеличивает вдвое вместимость вектора
	void PushBack(const Type& item) {
		if (size >= capacity)
		{
			if (!capacity) {
				capacity++;
			}
			ArrayPtr<Type> new_array(capacity * 2);
			std::copy(begin(), end(), new_array.Get());
			pointer.swap(new_array);
			capacity *= 2;
		}
		pointer[size] = item;
		size++;
	}
	// "Удаляет" последний элемент вектора. Вектор не должен быть пустым
	void PopBack() noexcept {
		if (!IsEmpty())
			size--;
	}
  Iterator Insert(ConstIterator pos, const Type& value) {
	  Iterator it = const_cast<Iterator>(pos);
	 if (capacity > size)
	 {
		 std::copy_backward(it, end(), it + 1);
		 *it = value;
		 ++size;
		 return it;
	 } else {
		 SimpleVector<Type> tmp(2 * capacity);
		 tmp.Resize(size + 1);
		 std::copy(begin(), it, tmp.begin());
		 auto position = std::distance(begin(), it);
		 tmp[position] = value;
		 std::copy(it, end(), &tmp[position + 1]);
		 swap(tmp);
		 return begin() + position;
	}
  }
  Iterator Erase(ConstIterator pos) {
	  auto distance = pos - begin();
	  std::copy(begin() + distance + 1, end(), begin() + distance);
	  size--;
	  return &pointer[distance];
  }
  void swap(SimpleVector& other) noexcept {
	  pointer.swap(other.pointer);
	  std::swap(size, other.size);
	  std::swap(capacity, other.capacity);
  }
  SimpleVector& operator=(const SimpleVector& rhs) {
	  if (this == &rhs) {
		  return *this;
	  }
	  if (rhs.IsEmpty()) {
		  Clear();
			return *this;
	  }
	  SimpleVector tmp(rhs);
		swap(tmp);
		return *this;
  }
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
	return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return !(lhs == rhs);
}
template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return (lhs<rhs)||(lhs==rhs);
}
template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return !(lhs<=rhs);
}
template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
	return !(lhs<rhs);
}

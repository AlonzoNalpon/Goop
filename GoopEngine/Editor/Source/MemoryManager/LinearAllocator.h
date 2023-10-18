/*!************************************************************************
\file Linearlocator.h
\author Han Qin Ding

\brief
This file functions for a linear Allocator.
The allocator works like a stack, whenever a system request for dynamic memory, it
will take from the top. Faster allocator but you have to be careful when it comes to getting and free memory from this allocator
*This means that the order of deallocation from the allocator shoud be opposite of the order of allocation
[ can use this for particle system in the future ]
**************************************************************************/
#pragma once
#include "BaseAllocator.h"
#include <pch.h>

class LinearAllocator : public BaseAllocator
{
public:


	//Having two instances that manage the same underlying memory can lead to conflicts and unexpected behavior.
	// So we dont not allow copy construct and copy assignemnt for allocator class
	LinearAllocator(const  LinearAllocator&) = delete;
	LinearAllocator& operator=(LinearAllocator&) = delete;

	LinearAllocator() = default;

	~LinearAllocator() = default;

	template<typename T, std::size_t sz = 1>
	T* Reserve(const std::initializer_list<T>& initList = {});

	template<typename T>
	T* Reserve(T& object);

	template<typename T>
	T* Reserve(const T& object);

	template<typename T>
	void Return(T*& ptr) noexcept;

	virtual void Initialise(std::size_t sz) noexcept;
	virtual void Free() override;
	virtual void PrintDetails() override;
	virtual void PrintMemLeak() override;


protected:

	void* m_current{};
};


/************************************************************************/ /*!
\ brief
My replacement for c++ new[] function (Allocate storage space for array)
	Tried to squeeze 3 different overloads into 1 function definition

	1.  Reserve<int, size>()
		- all the objects in the array will be default constructed

	2.  Reserve<int>(initializerList)
		- size of array  equal to size of initializer_list.
		- Copy everything in init_list to array

	3.  Reserve<int, size>(initializerList)
		- ONLY DO THIS IF YOUR ACTUAL ARRAY IS BIGGER THAN THE INITIALIZER LIST
		- size of array equal to size entered in the "< >" angle braces.
		- Copy everything in init_list to array, any empty slots left in the array will be filled with default constructed object

\param [initList]const std::initializer_list<T>&
	reference to a initializer list

\return
 pointer to the start of the dynamic allocated memory

*/
/************************************************************************/
template<typename T, std::size_t sz>
T* LinearAllocator::Reserve(const std::initializer_list<T>& initList)
{
	std::size_t count = (sz < initList.size()) ? initList.size() : sz;
	std::size_t sizeRequired = count * sizeof(T);

	assert(sizeRequired > 0 && BaseAllocator::m_usedBytes + sizeRequired <= (BaseAllocator::m_size));

	T* front = reinterpret_cast<T*>(m_current);
	T* pos = front;
	T* end = pos + count;

	for (const T& obj : initList)
	{
		*(pos) = obj;
		++pos;
	}

	while (pos != end)
	{
		new (pos) T();
		++pos;
	}

	m_current = reinterpret_cast<char*>(m_current) + sizeRequired;
	++BaseAllocator::m_numAllocations;
	BaseAllocator::m_usedBytes += sizeRequired;

	return front;
}

/************************************************************************/ /*!
\ brief
my replacement for c++ new. The function will dynamically allocate memory for 1 object.

\param [object]T&
	reference to an object

\return
 pointer to the start of the dynamic allocated memory
*/
/************************************************************************/
template<typename T>
T* LinearAllocator::Reserve(T& object)
{
	std::size_t sizeRequired = sizeof(T);
	assert(sizeRequired > 0 && BaseAllocator::m_usedBytes + sizeRequired <= (BaseAllocator::m_size));

	T* front = reinterpret_cast<T*>(m_current);
	*(front) = object;

	m_current = reinterpret_cast<char*>(m_current) + sizeRequired;
	++BaseAllocator::m_numAllocations;
	BaseAllocator::m_usedBytes += sizeRequired;

	return front;
}


/************************************************************************/ /*!
\ brief
my replacement for c++ new. The function will dynamically allocate memory for 1 object.

\param [object]const T&
	const reference to an object

\return
 pointer to the start of the dynamic allocated memory
*/
/************************************************************************/
template<typename T>
T* LinearAllocator::Reserve(const T& object)
{
	std::size_t sizeRequired = sizeof(T);
	assert(sizeRequired > 0 && BaseAllocator::m_usedBytes + sizeRequired <= (BaseAllocator::m_size));

	T* front = reinterpret_cast<T*>(m_current);
	*(front) = object;

	m_current = reinterpret_cast<char*>(m_current) + sizeRequired;
	++BaseAllocator::m_numAllocations;
	BaseAllocator::m_usedBytes += sizeRequired;

	return front;
}


/************************************************************************/ /*!
\ brief
my replacement for c++ delete. The function will "free" any memory that was previously
allocated by the linear allocator

\param [ptr]T*&
	pointer to the start of the allocated memory

*/
/************************************************************************/
template<typename T>
void LinearAllocator::Return(T*& ptr) noexcept
{
	assert(m_current == ptr && ptr != nullptr);
	std::size_t arraySize = reinterpret_cast<char*>(m_current) - reinterpret_cast<char*>(ptr);

	for (T* pos{ ptr }; pos != m_current; ++pos)
	{
		pos->~T();
	}

	m_current = ptr;
	BaseAllocator::m_usedBytes += arraySize;
	--BaseAllocator::m_numAllocations;
	ptr = nullptr;

}


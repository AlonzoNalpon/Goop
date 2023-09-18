#pragma once
/*!************************************************************************
\file ListAllocator.hpp
\author Han Qin Ding

\brief
This file functions for a linked list Allocator.
The allocator uses a linked list to keep track of how many free memory chunks left in the allocator,
and the size of each free meory chunk
**************************************************************************/

#pragma once
#include "./BaseAllocator.h"
#include <initializer_list>
#include "../Debugger/ErrorLogger/ErrorLogger.h"
#include "../Debugger/Exception/Exception.h"
#include <sstream>
#include <Windows.h>
#include <iostream>

struct FreeBlock
{
	size_t m_size{};
	FreeBlock* m_next;
};


struct AllocatedBlock
{
	size_t m_size{};
};


class ListAllocator : public BaseAllocator
{
public:

	//Having two instances that manage the same underlying memory can lead to conflicts and unexpected behavior.
	// So we dont not allow copy construct and copy assignemnt for allocator class
	ListAllocator(const  ListAllocator&) = delete;
	ListAllocator& operator=(ListAllocator&) = delete;

	ListAllocator() = default;
	~ListAllocator() = default;

	template<typename T, std::size_t sz = 1>
	T* Reserve(const std::initializer_list<T>& initList = {});

	template<typename T>
	T* Reserve(T& object);

	template<typename T>
	T* Reserve(const T& object);

	template<typename T>
	void Return(T*& ptr) noexcept;

	void Initialise(std::size_t sz) noexcept;
	virtual void Free() override;

	virtual void PrintDetails() override;
	virtual void PrintMemLeak() override;


protected:

	FreeBlock* m_freeMemList{};
	char* m_memEnd{};
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
T* ListAllocator::Reserve(const std::initializer_list<T>& initList)
{

	//Determine the actual memory size needed (object + header)
	std::size_t count = (sz < initList.size()) ? initList.size() : sz;
	std::size_t objectSize = count * sizeof(T);
	std::size_t padding = ((sizeof(AllocatedBlock) + objectSize) % 8 == 0) ? 0 : 8 - ((sizeof(AllocatedBlock) + objectSize) % 8);
	std::size_t sizeRequired = sizeof(AllocatedBlock) + objectSize + padding;

	//Check if the Allocator has enough memory to borrow

	//assert(sizeRequired > 0 && BaseAllocator::m_usedBytes + sizeRequired <= (BaseAllocator::m_size));
	if (BaseAllocator::m_usedBytes + sizeRequired > (BaseAllocator::m_size)) {
			//GE::Debug::ErrorLogger::GetInstance().LogError("Not enough memory in the Allocator",false);
			throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR,"Not enough memory in the Allocator",ERRLG_FUNC,ERRLG_LINE);
	}

	if (count <= 0) {
		//GE::Debug::ErrorLogger::GetInstance().LogError("You have entered the wrong amount", false);
		throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR, "You have entered the wrong amount", ERRLG_FUNC, ERRLG_LINE);

	}
	
	std::cout << "NO PRON\n";

	//Traverse through the List to find the Best suitable Memory block
	FreeBlock* bestFit{ nullptr }, * prevBlock{ nullptr }, * currBlock{ m_freeMemList };

	//Memory blocks are arranged according to size (smallest at the front, largest at the back)
	while (currBlock != nullptr)
	{
		if (currBlock->m_size >= sizeRequired)
		{
			bestFit = currBlock;
			break;
		}
		prevBlock = currBlock;
		currBlock = currBlock->m_next;
	}


	if (!bestFit)	//Unable to find a fitting memory block 
	{
		throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR, "No Memory Blocks big enough to hold object(s)", ERRLG_FUNC, ERRLG_LINE);
		//return nullptr;
	}

	// Take out the memory chunk from the list 
	// Only take out the amount we need
	// The remainder will be place back into the list

	if (bestFit->m_size > sizeRequired) //Memory block size is larger that what we need, we will put back the leftover free memories.
	{
		FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(bestFit) + sizeRequired);
		newBlock->m_size = bestFit->m_size - (sizeRequired);

		if (prevBlock) //The memory block we chose is in the middle of the list, we will slot the remaining block back into the gap
		{
			FreeBlock* prev{ m_freeMemList }, * larger{ m_freeMemList };
			while (larger)
			{
				if (larger->m_size >= newBlock->m_size && larger != bestFit)
				{
					break;
				}
				prev = larger;
				larger = larger->m_next;
			}
			prev->m_next = newBlock;
			newBlock->m_next = larger;
		}
		else
		{
			m_freeMemList = newBlock; //The memory block we chose is at the front of the list, we will add the block back to the front
			newBlock->m_next = bestFit->m_next;
		}
	}

	else		//Memory block size is just nice, we will just reconnect the memory block in the list
	{
		if (prevBlock)
		{
			prevBlock->m_next = bestFit->m_next;  //The memory block we chose is in the middle of the list, we will close the gap
		}
		else
		{
			m_freeMemList = bestFit->m_next; //The memory block we chose is at the front of the list, we will just move the list up by one
		}
	}

	// Fill up the Allocated memory block with objects
	AllocatedBlock* head = reinterpret_cast<AllocatedBlock*>(bestFit);
	T* start{ reinterpret_cast<T*>(head + 1) }, * end{ start + count }, * pos{ start };

	//Fill up the memory block with objects from initializer_list
	for (const T& obj : initList)
	{
		*(pos) = obj;
		++pos;
	}

	// Initialize objects into the rest of the Memory Chunk
	while (pos != end)
	{
		new (pos) T();
		++pos;
	}

	head->m_size = sizeRequired;

	++BaseAllocator::m_numAllocations;
	BaseAllocator::m_usedBytes += sizeRequired;

	return start;
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
T* ListAllocator::Reserve(T& object)
{
	std::size_t padding = ((sizeof(AllocatedBlock) + sizeof(T)) % 8 == 0) ? 0 : 8 - ((sizeof(AllocatedBlock) + sizeof(T)) % 8);
	std::size_t sizeRequired = sizeof(AllocatedBlock) + sizeof(T) + padding;

	if (BaseAllocator::m_usedBytes + sizeRequired > (BaseAllocator::m_size)) {
		//GE::Debug::ErrorLogger::GetInstance().LogError("Not enough memory in the Allocator",false);
		throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR, "Not enough memory in the Allocator", ERRLG_FUNC, ERRLG_LINE);
	}

	if (sizeRequired <= 0) {
		//GE::Debug::ErrorLogger::GetInstance().LogError("You have entered the wrong amount", false);
		throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR, "You have entered the wrong amount", ERRLG_FUNC, ERRLG_LINE);

	}

	//Traverse through the List to find the Best suitable Memory block
	FreeBlock* bestFit{ nullptr }, * prevBlock{ nullptr }, * currBlock{ m_freeMemList };

	//Memory blocks are arranged according to size (smallest at the front, largest at the back)
	while (currBlock != nullptr)
	{
		if (currBlock->m_size >= sizeRequired)
		{
			bestFit = currBlock;
			break;
		}
		prevBlock = currBlock;
		currBlock = currBlock->m_next;
	}

	if (!bestFit) //Unable to find a fitting memory block 
	{
		throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR, "No Memory Blocks big enough to hold object(s)", ERRLG_FUNC, ERRLG_LINE);
		//return nullptr;
	}

	// Take out the memory chunk from the list 
	// Only take out the amount we need
	// The remainder will be place back into the list

	if (bestFit->m_size > sizeRequired) //Memmory block size is larger that what we need, we will put back the leftover free memories.
	{
		FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(bestFit) + sizeRequired);
		newBlock->m_size = bestFit->m_size - (sizeRequired);

		if (prevBlock) //The memory block we chose is in the middle of the list, we will slot the remaining block back into the gap
		{
			FreeBlock* prev{ m_freeMemList }, * larger{ m_freeMemList };
			while (larger)
			{
				if (larger->m_size >= newBlock->m_size && larger != bestFit)
				{
					break;
				}
				prev = larger;
				larger = larger->m_next;
			}
			prev->m_next = newBlock;
			newBlock->m_next = larger;
		}
		else
		{
			m_freeMemList = newBlock; //The memory block we chose is at the front of the list, we will add the block back to the front
			newBlock->m_next = bestFit->m_next;
		}
	}

	else //Memory block size is just nice, we will just reconnect the memory block in the list
	{
		if (prevBlock)
		{
			prevBlock->m_next = bestFit->m_next;  //The memory block we chose is in the middle of the list, we will close the gap
		}
		else
		{
			m_freeMemList = bestFit->m_next; //The memory block we chose is at the front of the list, we will just move the list up by one
		}
	}

	AllocatedBlock* head = reinterpret_cast<AllocatedBlock*>(bestFit);
	T* start = reinterpret_cast<T*>(head + 1);
	*start = object;

	++BaseAllocator::m_numAllocations;
	BaseAllocator::m_usedBytes += sizeRequired;
	head->m_size = sizeRequired;

	return start;
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
T* ListAllocator::Reserve(const T& object)
{
	std::size_t padding = ((sizeof(AllocatedBlock) + sizeof(T)) % 8 == 0) ? 0 : 8 - ((sizeof(AllocatedBlock) + sizeof(T)) % 8);
	std::size_t sizeRequired = sizeof(AllocatedBlock) + sizeof(T) + padding;

	if (BaseAllocator::m_usedBytes + sizeRequired > (BaseAllocator::m_size)) {
		//GE::Debug::ErrorLogger::GetInstance().LogError("Not enough memory in the Allocator",false);
		throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR, "Not enough memory in the Allocator", ERRLG_FUNC, ERRLG_LINE);
	}

	if (sizeRequired <= 0) {
		//GE::Debug::ErrorLogger::GetInstance().LogError("You have entered the wrong amount", false);
		throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR, "You have entered the wrong amount", ERRLG_FUNC, ERRLG_LINE);

	}

	//Traverse through the List to find the Best suitable Memory block
	FreeBlock* bestFit{ nullptr }, * prevBlock{ nullptr }, * currBlock{ m_freeMemList };

	//Memory blocks are arranged according to size (smallest at the front, largest at the back)
	while (currBlock != nullptr)
	{
		if (currBlock->m_size >= sizeRequired)
		{
			bestFit = currBlock;
			break;
		}
		prevBlock = currBlock;
		currBlock = currBlock->m_next;
	}

	if (!bestFit) //Unable to find a fitting memory block 
	{
		std::cout << "FAILED\n";
		return nullptr;
	}

	// Take out the memory chunk from the list 
	// Only take out the amount we need
	// The remainder will be place back into the list

	if (bestFit->m_size > sizeRequired) //Memmory block size is larger that what we need, we will put back the leftover free memories.
	{
		FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(bestFit) + sizeRequired);
		newBlock->m_size = bestFit->m_size - (sizeRequired);

		if (prevBlock) //The memory block we chose is in the middle of the list, we will slot the remaining block back into the gap
		{
			FreeBlock* prev{ m_freeMemList }, * larger{ m_freeMemList };
			while (larger)
			{
				if (larger->m_size >= newBlock->m_size && larger != bestFit)
				{
					break;
				}
				prev = larger;
				larger = larger->m_next;
			}
			prev->m_next = newBlock;
			newBlock->m_next = larger;
		}
		else
		{
			m_freeMemList = newBlock; //The memory block we chose is at the front of the list, we will add the block back to the front
			newBlock->m_next = bestFit->m_next;
		}
	}

	else //Memory block size is just nice, we will just reconnect the memory block in the list
	{
		if (prevBlock)
		{
			prevBlock->m_next = bestFit->m_next;  //The memory block we chose is in the middle of the list, we will close the gap
		}
		else
		{
			m_freeMemList = bestFit->m_next; //The memory block we chose is at the front of the list, we will just move the list up by one
		}
	}

	AllocatedBlock* head = reinterpret_cast<AllocatedBlock*>(bestFit);
	T* start = reinterpret_cast<T*>(head + 1);
	*start = object;

	++BaseAllocator::m_numAllocations;
	BaseAllocator::m_usedBytes += sizeRequired;
	head->m_size = sizeRequired;

	return start;
}


/************************************************************************/ /*!
\ brief
my replacement for c++ delete. The function will "free" any memory that was previously
allocated by the linked list allocator

\param [ptr]T*&
	pointer to the start of the allocated memory

*/
/************************************************************************/
template<typename T>
void ListAllocator::Return(T*& ptr) noexcept
{
	//Check if the pointer is null
	if (ptr == nullptr)
	{
		GE::Debug::ErrorLogger::GetInstance().LogWarning("You have either already freed this memory or you are trying to delete a nullptr\n", false);
		return;
	}

	//Check if the memory block belongs to this Allocator
	char* checkFront = reinterpret_cast<char*>(ptr);
	if (checkFront<BaseAllocator::m_start || checkFront > m_memEnd)
	{
		GE::Debug::ErrorLogger::GetInstance().LogWarning("THIS MEMORY BLOCK DOES NOT BELONG TO THIS ALLOCATOR\n", false);
		return;
	}

	//Check if the memory block has already been freed
	FreeBlock* cur = m_freeMemList;
	while (cur)
	{
		char* curChar = reinterpret_cast<char*>(cur);
		if (curChar == checkFront)
		{
			GE::Debug::ErrorLogger::GetInstance().LogWarning("You have already freed this memS\n", false);
			return;
		}
		if (curChar < checkFront && curChar + (cur->m_size) > checkFront)
		{
			GE::Debug::ErrorLogger::GetInstance().LogWarning("You have already freed this mem\n", false);
			return;
		}
		cur = cur->m_next;
	}

	//Get the size of the memory block and the number of objects within it
	AllocatedBlock* head = reinterpret_cast<AllocatedBlock*>(ptr) - 1;
	size_t sizeRequired{ head->m_size }, count{ (sizeRequired - sizeof(AllocatedBlock) - ((sizeRequired - sizeof(AllocatedBlock)) % sizeof(T))) / sizeof(T) };

	//Deconstruct all the objects within the memory block;
	T* obj = ptr;
	for (size_t i{ 0 }; i < count; ++i)
	{
		obj->~T();
	}

	//Merge the Memory block with any of the blocks within the list;
	char* front = reinterpret_cast<char*>(head);
	FreeBlock* start{ reinterpret_cast<FreeBlock*>(head) }, * end{ reinterpret_cast<FreeBlock*>(reinterpret_cast<char*>(head) + sizeRequired) };
	FreeBlock* before{ nullptr }, * befPrev{ nullptr };
	FreeBlock* after{ nullptr }, * aftPrev{ nullptr };
	FreeBlock* prev{ nullptr };
	cur = m_freeMemList;

	//Loop through the list to check for any adjacent memory block
	while (cur && !(before && after))
	{
		aftPrev = (cur == end) ? prev : aftPrev;
		befPrev = ((reinterpret_cast<char*>(cur) + cur->m_size) == front) ? prev : befPrev;

		after = (cur == end) ? cur : after;
		before = ((reinterpret_cast<char*>(cur) + cur->m_size) == front) ? cur : before;

		prev = cur;
		cur = cur->m_next;
	}

	if (after) //we can merge one of the block to the back of the current block;
	{
		start->m_size += after->m_size;
		after->m_next = (after->m_next) ? (after->m_next == before) ? (after->m_next)->m_next : after->m_next : after->m_next;
		if (aftPrev)
		{
			aftPrev->m_next = after->m_next;
		}

		else
		{
			m_freeMemList = after->m_next;
		}

	}


	if (before) //we can merge the current block to the back of another block
	{
		before->m_size = before->m_size + start->m_size;
		before->m_next = (before->m_next) ? (before->m_next == after) ? (before->m_next)->m_next : before->m_next : before->m_next;
		if (befPrev)
		{
			befPrev->m_next = before->m_next;
		}

		else
		{
			m_freeMemList = before->m_next;
		}
		start = before;
	}

	//Update the relevant values 
	BaseAllocator::m_usedBytes -= sizeRequired;
	--BaseAllocator::m_numAllocations;

	//Slot the memory back into the list
	cur = m_freeMemList;
	prev = nullptr;
	while (cur)
	{
		if (cur->m_size >= start->m_size)
		{
			if (prev)
			{
				prev->m_next = start;
			}

			else
			{
				m_freeMemList = start;
			}
			start->m_next = cur;
			return;		//We will return straight away after finding a slot in the list
		}
		prev = cur;
		cur = cur->m_next;
	}

	if (prev)	//We reached the end of the list and we cant find a slot. so we will just append this memory at the back of the list
	{
		prev->m_next = start;
	}

	else //The list itself is already empty, so we will just set this as the start of the list
	{
		m_freeMemList = start;
	}

	start->m_next = nullptr;
	ptr = nullptr;
}
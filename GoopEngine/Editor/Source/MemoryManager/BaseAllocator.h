#pragma once
/*!************************************************************************
\file BaseAllocator.h
\author Han Qin Ding

\brief
This file contains the base class for all allocators
The base class contains essential data members like Allocator sizee and Number of Allocation
**************************************************************************/

#pragma once
#include <cstddef>
#include <cstdint>


class BaseAllocator
{
public:


	//Having two instances that manage the same underlying memory can lead to conflicts and unexpected behavior.
	// So we dont not allow copy construct and copy assignemnt for allocator class
	BaseAllocator(const BaseAllocator&) = delete;
	BaseAllocator& operator=(BaseAllocator&) = delete;


	BaseAllocator() = default;

	~BaseAllocator() = default;

	virtual void Free() {};
	virtual void PrintDetails() {};
	virtual void PrintMemLeak() {};

	const std::size_t GetFree() const noexcept;
	const std::size_t& GetSize() const noexcept;
	const std::size_t& GetUsed() const noexcept;
	const std::size_t& GetNumAllocation() const noexcept;

protected:
	std::size_t m_size{};
	std::size_t m_usedBytes{};
	std::size_t m_align_padding{};
	std::size_t m_numAllocations{};


	void* m_start;
	void* m_originalStart;
};

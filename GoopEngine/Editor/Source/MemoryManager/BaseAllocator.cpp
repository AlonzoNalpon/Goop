#include <pch.h>
#include "./BaseAllocator.h"


const std::size_t& BaseAllocator::GetSize() const noexcept
{
	return m_size;
}

const std::size_t& BaseAllocator::GetUsed() const noexcept
{
	return m_usedBytes;
}


const std::size_t BaseAllocator::GetFree() const noexcept
{
	return (m_size - m_usedBytes);
}

const std::size_t& BaseAllocator::GetNumAllocation() const noexcept
{
	return m_numAllocations;
}


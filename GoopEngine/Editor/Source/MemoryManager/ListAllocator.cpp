
#include <pch.h>
#include "./ListAllocator.h"



void ListAllocator::Initialise(std::size_t sz) noexcept {
	BaseAllocator::m_originalStart = new void* [sz];
	size_t alignment = sizeof(double);
	m_start = std::align(alignment, alignment, m_originalStart, sz);
	m_memEnd = reinterpret_cast<char*>(BaseAllocator::m_start) + sz;
	m_freeMemList = static_cast<FreeBlock*>(BaseAllocator::m_start);
	m_freeMemList->m_next = nullptr;
	m_freeMemList->m_size = sz;
	BaseAllocator::m_size = sz;
	BaseAllocator::m_usedBytes = 0;
	BaseAllocator::m_numAllocations = 0;
	//std::cout << "Free Block Header Size: " << sizeof(FreeBlock) << "\n";
	//std::cout << "Allocated Block Header Size: " << sizeof(AllocatedBlock) << "\n";
}


void ListAllocator::PrintDetails() {

	std::cout << "List Allocator:\n";
	std::cout << "---------------------------------------------\n";
	std::cout << "Total Memory Size: " << BaseAllocator::m_size << " bytes\n";
	std::cout << "Memory Used	: " << BaseAllocator::m_usedBytes << " bytes\n";
	std::cout << "Memory left: " << BaseAllocator::GetFree() << " bytes\n";
	std::cout << "Number of Allocations: " << BaseAllocator::m_numAllocations << "\n";
	FreeBlock* block = m_freeMemList;
	int count{};
	std::cout << "LIST\n";
	while (block) {
		std::cout << "	Chunk " << count + 1 << ": " << block->m_size << "\n";
		++count;
		block = block->m_next;
	}
	std::cout << "----------------------------------------------\n";
}


void ListAllocator::PrintMemLeak()
{
	if (m_usedBytes != 0) { // We will print mem leak details if required
		OutputDebugStringA("\nList Allocator Mem Leak:\n");
		OutputDebugStringA("---------------------------------------------\n");
		std::string output1 = "Total Memory not freed	: " + std::to_string(BaseAllocator::m_usedBytes) + " bytes\n";
		std::string output2 = "Number of Allocations not free: " + std::to_string(BaseAllocator::m_numAllocations) + "\n";
		OutputDebugStringA(output1.c_str());
		OutputDebugStringA(output2.c_str());
		OutputDebugStringA("----------------------------------------------\n\n");
	}
}


void ListAllocator::Free() {
	BaseAllocator::m_start = nullptr;
	if (BaseAllocator::m_originalStart != nullptr) {
		delete[] BaseAllocator::m_originalStart;
	}
	
}
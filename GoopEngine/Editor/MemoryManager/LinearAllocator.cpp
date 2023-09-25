#include "LinearAllocator.h"
//#include <utility>
//#include <Windows.h>
//#include <sstream>
#include <pch.h>

/************************************************************************/ /*!
\ brief
Print out the details of the linear allocator.
e.g.( total size, total memory left )

*/
/************************************************************************/
void LinearAllocator::Initialise(std::size_t sz) noexcept 
{
	BaseAllocator::m_start = new void*[sz];
	m_current = BaseAllocator::m_start;
	BaseAllocator::m_size = sz;
	BaseAllocator::m_usedBytes = 0;
	BaseAllocator::m_numAllocations = 0;
}

/************************************************************************/ /*!
\ brief
Print out the details of the linear allocator.
e.g.( total size, total memory left )

*/
/************************************************************************/
void LinearAllocator::PrintDetails() {
	std::cout << "Linear Allocator:\n";
	std::cout << "---------------------------------------------\n";
	std::cout << "Total Memory Size: " << BaseAllocator::m_size << " bytes\n";
	std::cout << "Memory Used	: " << BaseAllocator::m_usedBytes << " bytes\n";
	std::cout << "Memory left: " << BaseAllocator::GetFree() << " bytes\n";
	std::cout << "Number of Allocations: " << BaseAllocator::m_numAllocations << "\n";
	std::cout << "----------------------------------------------\n";
}

/************************************************************************/ /*!
\ brief
Print out the details of memory leak in linear  allocator, if any

*/
/************************************************************************/
void LinearAllocator::PrintMemLeak() {



	if (m_usedBytes != 0) { // We will print mem leak details if required
		OutputDebugStringA("\nLinear Allocator Mem Leak:\n");
		OutputDebugStringA("---------------------------------------------\n");
		std::string output1 = "Total Memory not freed	: " + std::to_string(BaseAllocator::m_usedBytes) + " bytes\n";
		std::string output2 = "Number of Allocations not free: " + std::to_string(BaseAllocator::m_numAllocations) + "\n";
		OutputDebugStringA(output1.c_str());
		OutputDebugStringA(output2.c_str());
		OutputDebugStringA("----------------------------------------------\n\n");
	}
}

void LinearAllocator::Free() {
	delete[] BaseAllocator::m_start;
}
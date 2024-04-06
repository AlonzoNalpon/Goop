/*!************************************************************************
\file MemoryManager.cpp
\author Han Qin Ding
\date   25-September-2023

\brief
This file contains functions for a memory manager system.
You can allocate/deallocate dynamic memory through this memory manager.
You can also print out details of each of the allocators.
By default, we will be using our custom allocator. (not std::allocator)

Functions you can use:

	- InitializeAllAlocators();
		initialize all the allocators in the memory manager

	- FreeAllAlocators();
		Free all the memories that were allocated in the memory manager

	- PrintAllDetails();
		Print out the details of all of the allocators in the memory manager

	- void ChangeAllocator(bool useCustomAllocator);
		In the event that the custom allocator does not work, you can call this
		function right after InitializeAllAlocators() to switch to std::allocator.
		You can proceed to use the other functions as normal

		template<typename T, std::size_t sz >
	- T* Reserve(const std::initializer_list<T>& initList);
		Call this function you want to allocate a memory for an array of objects and you already have an initializer list.
		Enter the object type in the < >

		template<typename T, std::size_t sz >
	- T* Reserve(const std::initializer_list<T>& initList);
		Call this function you want to allocate a memory for an array of objects but you only know the array size.
		Enter the object type and the array size in the < >

		template<typename T>
	- T* Reserve(T& obj);
		Call this function you want to allocate a memory 1 lvalue object.
		Enter the object type in the < >

		template<typename T>
	- T* Reserve(const T& obj);
		 Call this function you want to allocate a memory 1 rvalue object.
		 Enter the object type in the < >

		template<typename T, ALLOCATOR_TYPE allocatorType = LIST_ALLOCATOR>
	- void Return(T*&);
		Call this function when you want to free a dynamic allcoated memory
		you can just pass in the pointer into the function param
		IF YOU PASS IN AN INVALID MEMORY OR A MEMORY THATS NOT ALLOCATED BY THIS MEMORY MANAGER, IT WILL PRINT OUT ERROR MESSAGE



EXAMPLE ON HOW TO ALLOCATE:

			[ Allocate memory for single object ]
	 1. int* ptr = Reserve<int>( obj );  // allocating memory for 1 int object

			[ Allocate memory for an array ( using initializer list ) ]
	 2. int* array = Reserve<int>( initializer_list );  // allocating memory for an array filled with objects from initializer list

			[ Allocate memory for an array ( using array size ) ]
	 3. int* array = Reserve< int, 20 >( );  // allocating memory for an array of size 20. All objects inside are default constructed


EXAMPLE ON HOW TO DEALLOCATE:

			[ Deallocate memory for any memory that was dynamically allocated by memory manager ]
	 1. Return( ptr );  // You just need to pass in the pointer, the allocator will know if its an array or a single object

**************************************************************************/
#include <pch.h>
#include "./MemoryManager.h"
#include <chrono>
#include <algorithm>
#include <Windows.h>
#include <sstream>


using namespace GE::Memory;


void MemoryManager::InitializeAllAlocators(int listSize)
{
	g_listAllocator.Initialise(listSize);

}


void MemoryManager::FreeAllAlocators()
{
	if (g_useMyAllocator)
	{
		g_listAllocator.PrintMemLeak();
	}
	else
	{
		PrintMemLeak();
	}
	g_listAllocator.Free();
	//for (void* v : g_arrayMem) {
	//	delete[] v;
	//}
	//for (void* v : g_objMem) {
	//	delete v;
	//}
}



void MemoryManager::PrintAllDetails()
{
	g_listAllocator.PrintDetails();
}


void MemoryManager::ChangeAllocator(bool useCustomAllocator)
{
	g_useMyAllocator = useCustomAllocator;
}

void MemoryManager::AddMemStd(void* ptr, bool isArray)
{
	if (isArray)
	{
		g_arrayMem.push_back(ptr);
	}
	else
	{
		g_objMem.push_back(ptr);
	}
}

bool MemoryManager::isArrayMem(void* ptr)
{

	//Check if the ptr is an Array ptr
	std::vector<void*>::iterator it{ std::find(g_arrayMem.begin(), g_arrayMem.end(), ptr) };

	if (it != g_arrayMem.end())
	{
		g_arrayMem.erase(it);
		return true;
	}

	it = std::find(g_objMem.begin(), g_objMem.end(), ptr);
	if (it != g_objMem.end()) {
		g_objMem.erase(it);
	}
	else {
		throw GE::Debug::Exception<MemoryManager>(GE::Debug::LEVEL_WARN, "This memory was not allocated by memory manager", ERRLG_FUNC, ERRLG_LINE);
	}
	
	
	return false;
}


void MemoryManager::PrintMemLeak()
{
	if ((g_arrayMem.size() != 0) || (g_objMem.size() != 0))
	{
		OutputDebugStringA("\n std::Allocator Mem Leak:\n");
		OutputDebugStringA("---------------------------------------------\n");
		if (g_arrayMem.size() != 0)
		{ // We will print mem leak details if required
			std::string output = "Number of Arrays not free: " + std::to_string(g_arrayMem.size()) + "\n";
			OutputDebugStringA(output.c_str());
		}
		if (g_objMem.size() != 0)
		{ // We will print mem leak details if required
			std::string output = "Number of Objects not free: " + std::to_string(g_objMem.size()) + "\n";
			OutputDebugStringA(output.c_str());
		}
		OutputDebugStringA("----------------------------------------------\n\n");
	}
}

#ifdef _DEBUG
void MemoryManager::TestAllAllocators()
{
	MemoryManager* mm = &MemoryManager::GetInstance();
	auto startTime = std::chrono::high_resolution_clock::now();
	std::cout << "\nAllocator Test\n";
	std::cout << "-------------------------------------------------\n";
	int* testint = mm->Reserve(1);
	double* testd = mm->Reserve<double>(1.0);
	std::cout << "Testing if the Allocator checks the validity of the value entered into the Reserve function:\n";
	TestObject* testt = mm->Reserve<TestObject, 0>();
	testt = mm->Reserve<TestObject, 100>();

	auto endTime = std::chrono::high_resolution_clock::now();

	auto first_duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

	//auto startTime2 = std::chrono::high_resolution_clock::now();
	//int* testint2 = new int(1);
	//double* testd2 = new double(1.0);
	//TestObject* testt2 = new TestObject[100];
	//auto endTime2 = std::chrono::high_resolution_clock::now();
	//auto second_duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime2 - startTime2);
	// << "Time needed for std::allocator to allocate memory (Mircoseconds)" << second_duration.count() << "\n\n\n";


	std::cout << "\nTime needed for my allocator to allocate memory (Mircoseconds)" << first_duration.count() << "\n";
	mm->PrintAllDetails();

	std::cout << "Value of the 91st object in the array: " << testt[90].m_testInt << "\n";

	//Test the Memory Return Functions 
	int* nope = new int();
	mm->Return(testint);
	mm->Return(testd);
	mm->Return(testt);
	std::cout << "Testing if the Allocator checks the validity of the value entered into the Return function:\n";
	mm->Return(testt); //Try to return a memory that is already freed
	mm->Return(nope); //Try to free a memory that does not belong to the allocator
	mm->PrintAllDetails();
	delete nope;
}
#endif // _DEBUG
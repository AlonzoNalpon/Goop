#pragma once
#pragma once
/*!************************************************************************
\file MemoryManager.h
\author Han Qin Ding

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
#include "./BaseAllocator.h"
#include "./ListAllocator.h"
#include "../Singleton/Singleton.h"
#include <random>





namespace GE
{
	namespace Memory
	{

		enum ALLOCATOR_TYPE
		{
			LIST_ALLOCATOR = 0,
			LINEAR_ALLOCATOR,
			ALLOCATOR_TOTAL
		};

		struct TestPoint
		{
			double m_x{ 1 }, m_y{ 2 };
		};

		struct TestObject
		{
			TestPoint m_testPoint{};
			TestPoint* m_testPointer{};
			double m_testDouble{};
			float m_testfloat{};
			int m_testInt{};
			char m_testChar{};

			~TestObject()
			{
				//std::cout << "DESTROYED\n";
			}
			TestObject()
			{
				std::random_device rd;  // Seed the engine with a random device
				std::mt19937 gen(rd()); // Mersenne Twister engine
				//std::cout << "Created\n";
				// Define a distribution for generating integers between 1 and 100 (inclusive)
				std::uniform_int_distribution<int> dis(1, 100);
				m_testInt = dis(gen);
			}
		};

		class MemoryManager : public Singleton<MemoryManager> {

			ListAllocator g_listAllocator;
			bool g_useMyAllocator{ true };
			std::vector<void*>g_arrayMem;
			std::vector<void*>g_objMem;

		public:
			~MemoryManager() {
				if (g_useMyAllocator)
				{
					g_listAllocator.PrintMemLeak();
				}
				else
				{
					PrintMemLeak();
				}
				g_listAllocator.Free();
			}
			/************************************************************************/ /*!
			\ brief
			Initialize all the Allocators in the memory manager.
			*/
			/************************************************************************/
			void InitializeAllAlocators(int listSize);

			/************************************************************************/ /*!
			\ brief
			Free all the Allocators in the memory manager.
			*/
			/************************************************************************/
			void FreeAllAlocators();

			/************************************************************************/ /*!
			\ brief
			Print out the details of all the Allocators in the memory manager
			*/
			/************************************************************************/
			void PrintAllDetails();


			/************************************************************************/ /*!
			\ brief
			This function will be called if you want to change the allocator we are using.
			In the event that the custom allocator does not work properly, we will go back to std::allocator instead

			\param [useCustomAllocator] bool
				bool variable to determine whether to use our custom allocator or go back to std::allocator (new, delete)
			*/
			/************************************************************************/
			void ChangeAllocator(bool useCustomAllocator);

			/************************************************************************/ /*!
			\ brief
			helper function for allocating memory with std::allocator.
			keep records of whether the allocated memory is for an array or for an object

			\param [ptr]void*
				pointer to the allocated memory

			\param [isArray] bool
				bool variable to determine whether to the memory is for an array or an object
			*/
			/************************************************************************/
			void AddMemStd(void* ptr, bool isArray = false);

			/************************************************************************/ /*!
			\ brief
			Checks if the memory is for an array or an object

			\param [ptr]void*
				pointer to the allocated memory

			\return
			 bool variable to determine whether to the memory is for an array or an object
			*/
			/************************************************************************/
			bool isArrayMem(void* ptr);

			/************************************************************************/ /*!
			\ brief
			my replacement for c++ new []. The function will dynamically allocate memory an array of objects.
			the objects in the initializer list will eb pass into the array.

			\param [initList]const std::initializer_list<T>&
				reference to a initializer list

			\return
			 pointer to the start of the dynamic allocated memory
			*/
			/************************************************************************/
			template<typename T, std::size_t sz = 1, ALLOCATOR_TYPE allocatorType = LIST_ALLOCATOR>
			T* Reserve(const std::initializer_list<T>& initList)
			{
				if (g_useMyAllocator == true)
				{
					try {
						return g_listAllocator.Reserve<T, sz>(initList);
					}
					catch (GE::Debug::IExceptionBase& e) {
						e.LogSource();
						e.Log();
					}
				}
				std::size_t sizeRequired = (sz > initList.size()) ? sz : initList.size();

				if (sizeRequired <= 1) {
					//GE::Debug::ErrorLogger::GetInstance().LogError("You have entered the wrong amount", false);
					throw GE::Debug::Exception<ListAllocator>(GE::Debug::LEVEL_ERROR, "You have entered the wrong amount", ERRLG_FUNC, ERRLG_LINE);
				}
				if (sizeRequired == 1)
				{
					T* mem = new T();
					AddMemStd(reinterpret_cast<void*>(mem));
					return mem;
				}
				T* mem = new T[sizeRequired];
				AddMemStd(reinterpret_cast<void*>(mem), true);
				return mem;

			}


			/************************************************************************/ /*!
			\ brief
			my replacement for c++ new []. The function will dynamically allocate memory an array of objects.

			\return
			 pointer to the start of the dynamic allocated memory
			*/
			/************************************************************************/
			template<typename T, std::size_t sz = 1, ALLOCATOR_TYPE allocatorType = LIST_ALLOCATOR>
			T* Reserve()
			{
				if (g_useMyAllocator)
				{
					try {
						return g_listAllocator.Reserve<T, sz>();
					}
					catch (GE::Debug::IExceptionBase& e) {
						e.LogSource();
						e.Log();
						return nullptr;
					}
				}
				std::size_t sizeRequired = sz;

				if (sizeRequired < 1) {
					GE::Debug::ErrorLogger::GetInstance().LogError("You have entered the wrong amount", false);
					//throw GE::Debug::Exception<MemoryManager>(GE::Debug::LEVEL_ERROR, "You have entered the wrong amount", ERRLG_FUNC, ERRLG_LINE);
					return nullptr;
				}
				if (sizeRequired == 1)
				{
					T* mem = new T();
					AddMemStd(reinterpret_cast<void*>(mem));
					return mem;
				}
				T* mem = new T[sizeRequired];
				AddMemStd(reinterpret_cast<void*>(mem), true);
				return mem;

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
			template<typename T, ALLOCATOR_TYPE allocatorType = LIST_ALLOCATOR>
			T* Reserve(T& object)
			{
				if (g_useMyAllocator)
				{
					try {
						return g_listAllocator.Reserve<T>(object);
					}
					catch (GE::Debug::IExceptionBase& e) {
						e.LogSource();
						e.Log();
					}
				
				}
				T* mem = new T(object);
				AddMemStd(reinterpret_cast<void*>(mem));
				return mem;
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
			template<typename T, ALLOCATOR_TYPE allocatorType = LIST_ALLOCATOR>
			T* Reserve(const T& object)
			{
				if (g_useMyAllocator)
				{
					try {
						return g_listAllocator.Reserve<T>(object);
					}
					catch (GE::Debug::IExceptionBase& e) {
						e.LogSource();
						e.Log();
					}

				}
				T* mem = new T(object);
				AddMemStd(reinterpret_cast<void*>(mem), false);
				return mem;
			}

			/************************************************************************/ /*!
			\ brief
			my replacement for c++ delete. The function will "free" any memory that was previously
			allocated by this memory manager.

			\param [ptr]T*&
				pointer to the start of the allocated memory

			*/
			/************************************************************************/
			template<typename T, ALLOCATOR_TYPE allocatorType = LIST_ALLOCATOR>
			void Return(T*& ptr) noexcept
			{
				if (g_useMyAllocator == true)
				{
						g_listAllocator.Return(ptr);
						return;	
				}
				try {
					if (isArrayMem(reinterpret_cast<void*>(ptr)))
					{
						delete[] ptr;
					}
					else
					{
						delete ptr;
					}
				}
				catch (GE::Debug::IExceptionBase& e) {
					e.LogSource();
					e.Log();
				}
				
			}


			/************************************************************************/ /*!
			\ brief
			Prints out the details of the memory leaks if any.
			*/
			/************************************************************************/
			void PrintMemLeak();


			/************************************************************************/ /*!
			\ brief
			This function is used for testing all the allocators in the memory manager.
			It will put test objects into each allocator to test if they are working fine
			(this function is used for testing and debugging)
			*/
			/************************************************************************/
			void TestAllAllocators();
		};

	}
}

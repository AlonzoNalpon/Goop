/*!
@file    ObjectAllocator.cpp
@author  han.q@digipen.edu
@date    01/30/2024

This file implements functionality useful and necessary to build a simple memory Manager.
Each Object Allocator allocates dynamic memory and store them as pages.
The free memories are keep tracked using the free list.
User will use the object allcoator to allocacte and free dynamic memory

*//*__________________________________________________________________________*/
#include <pch.h>
#include "ObjectAllocator.h"



void ObjectAllocator::AllocatePage()
{
	//std::cout << "NEW PAGE \n";
	GenericObject* latestPage{ PageList_ };
	/*************************************/
	// Allocate Memory
	/*************************************/
	try
	{
		GenericObject* newPage = reinterpret_cast<GenericObject*>(new char[m_oaStats.PageSize_]);
		if (latestPage)
		{
			GenericObject* tail = PageList_;
			latestPage = PageList_ = newPage;
			newPage->Next = tail;
		}
		else
		{
			latestPage = PageList_ = newPage;
			PageList_->Next = nullptr;
		}

	}
	catch (std::bad_alloc&)
	{
		throw GE::Debug::Exception<ObjectAllocator>(GE::Debug::LEVEL_ERROR, ErrMsg("allocate_new_page: No system memory available."));
	}

	char* start = reinterpret_cast<char*>(latestPage) + (sizeof(GenericObject) + m_oaConfig.LeftAlignSize_);

	/*************************************/
	// Cast each block to Generic Object 
	// Add each GenObj into FreeList
	/*************************************/
	for (unsigned i{ 0 }; i < m_oaConfig.ObjectsPerPage_; ++i)
	{
		GenericObject* newBlock = reinterpret_cast<GenericObject*>(start + m_oaConfig.PadBytes_);
		newBlock->Next = nullptr;
		if (!FreeList_)
			FreeList_ = newBlock;
		else
		{
			newBlock->Next = FreeList_;
			FreeList_ = newBlock;
		}
		start += m_blockSize;
	}
	m_oaStats.FreeObjects_ += m_oaConfig.ObjectsPerPage_;
	m_oaStats.PagesInUse_++;
}

ObjectAllocator::ObjectAllocator(size_t ObjectSize) : PageList_{ nullptr }, FreeList_{ nullptr }, m_oaConfig{}, m_oaStats{}
{
	
	m_oaStats.ObjectSize_ = (ObjectSize >= sizeof(GenericObject))? ObjectSize: sizeof(GenericObject);

	// Default size 
	size_t totalSizeReq{ (m_oaStats.ObjectSize_ * m_oaConfig.ObjectsPerPage_) + sizeof(GenericObject) };
	if (m_oaConfig.Alignment_ > 1)
	{
		m_oaConfig.LeftAlignSize_ = static_cast<unsigned>(m_oaConfig.Alignment_ - ((sizeof(GenericObject) + m_oaConfig.PadBytes_) % m_oaConfig.Alignment_));
		m_oaConfig.InterAlignSize_ = static_cast<unsigned>(m_oaConfig.Alignment_ - ((m_oaStats.ObjectSize_ + (m_oaConfig.PadBytes_ * 2)) % m_oaConfig.Alignment_));
	}


		//Add the size of the paddings and alignments 
	totalSizeReq += (m_oaConfig.ObjectsPerPage_ * 2 * m_oaConfig.PadBytes_)
		+ ((m_oaConfig.ObjectsPerPage_ - 1) * m_oaConfig.InterAlignSize_)
		+ m_oaConfig.LeftAlignSize_;

	m_blockSize = m_oaStats.ObjectSize_ + (2 * m_oaConfig.PadBytes_) + m_oaConfig.InterAlignSize_;

	//Set the value of the OAStats
	m_oaStats.PageSize_ = totalSizeReq;
	std::cout << "NEW OBJ ALLOC FOR COMPONENT :" << m_oaStats.ObjectSize_ << "\n";
	if (!m_oaConfig.UseCPPMemManager_)
	{
		try
		{
			AllocatePage();
		}
		catch (GE::Debug::Exception<ObjectAllocator> e)
		{
			throw e;
		}
	}
}

ObjectAllocator::~ObjectAllocator()
{
	std::cout << "DTOR\n";
	GenericObject* head = PageList_;
	while (head)
	{
		GenericObject* temp = head;
		head = head->Next;
		char* toFree = reinterpret_cast<char*>(temp);
		delete[] toFree;
	}
}

// Take an object from the free list and give it to the client (simulates new)
// Throws an exception if the object can't be allocated. (Memory allocation problem)
void* ObjectAllocator::Allocate(const char* label)
{

	if (!m_oaConfig.UseCPPMemManager_)
	{
		if (m_oaStats.FreeObjects_ == 0)
		{
			if (m_oaStats.PagesInUse_ == m_oaConfig.MaxPages_)
				throw GE::Debug::Exception<ObjectAllocator>(GE::Debug::LEVEL_ERROR, ErrMsg("Allocate memory, no more pages left"));

			try
			{
				AllocatePage();
			}
			catch (GE::Debug::Exception<ObjectAllocator> e)
			{
				throw e;
			}
		}

		//GenericObject* test = FreeList_;

		//std::cout << "\n+++++++++++++++++++++++++++++++++++++++++++++\n";
		//std::cout << "(b4) ALLOC FOR " << m_oaStats.ObjectSize_ << "\n";
		//while (test)
		//{
		//		std::cout << "Block: " << test << "-> " << test->Next << "\n";
		//	test = test->Next;
		//}


		GenericObject* memBlock = FreeList_;
		FreeList_ = FreeList_->Next;



		//test = FreeList_;
		//std::cout << "\n(AFT) ALLOC FOR " << m_oaStats.ObjectSize_ << "\n";
		//while (test)
		//{
		//	std::cout << "Block: " << test << "-> " << test->Next << "\n";
		//	test = test->Next;
		//}
		//std::cout << "+++++++++++++++++++++++++++++++++++++++++++++\n";

		m_oaStats.ObjectsInUse_++;
		m_oaStats.Allocations_++;
		m_oaStats.FreeObjects_--;
		m_oaStats.MostObjects_ = (m_oaStats.MostObjects_ < m_oaStats.ObjectsInUse_) ? m_oaStats.ObjectsInUse_ : m_oaStats.MostObjects_;

		return memBlock;
	}


	//Use CPP memory manager
	char* memBlock{ nullptr };
	try {
		memBlock = new char[m_oaStats.ObjectSize_];
	}
	catch (std::bad_alloc&)
	{
		throw GE::Debug::Exception<ObjectAllocator>(GE::Debug::LEVEL_ERROR, ErrMsg("Use CPP Memory Manager, No system memory available."));
	}


	m_oaStats.Allocations_++;
	m_oaStats.ObjectsInUse_++;
	m_oaStats.MostObjects_ = (m_oaStats.MostObjects_ < m_oaStats.ObjectsInUse_) ? m_oaStats.ObjectsInUse_ : m_oaStats.MostObjects_;
	return memBlock;

}

// Returns an object to the free list for the client (simulates delete)
// Throws an exception if the the object can't be freed. (Invalid object)
void ObjectAllocator::Free(void* Object)
{
	if (!m_oaConfig.UseCPPMemManager_)
	{
		if (!Object)
			throw GE::Debug::Exception<ObjectAllocator>(GE::Debug::LEVEL_ERROR, ErrMsg("Freeing a nullptr"));

		GenericObject* toFreeBlock = reinterpret_cast<GenericObject*>(Object);
		bool IsDoubleFree{ false };
		GenericObject* FreeListHead = FreeList_;
		//GenericObject* test = FreeList_;
		
		//std::cout << "\n-------------------------------------------\n";
		//std::cout << "(b4) FREE FOR " << m_oaStats.ObjectSize_ << "\n";
		//while (test)
		//{
		//	std::cout << "Block: " << test << "-> " << test->Next << "\n";
		//	test = test->Next;
		//}




		while (FreeListHead)
		{
			IsDoubleFree = (FreeListHead == toFreeBlock) ? true : IsDoubleFree;
			FreeListHead = FreeListHead->Next;
		}
		if (IsDoubleFree)
			throw GE::Debug::Exception<ObjectAllocator>(GE::Debug::LEVEL_ERROR, ErrMsg("double freeing"));

		bool IsOutside{ true };
		GenericObject* PageHead = PageList_;
		GenericObject* ActualPage{ nullptr };
		while (PageHead)
		{
			IsOutside = (toFreeBlock >= PageHead && reinterpret_cast<char*>(toFreeBlock) < (reinterpret_cast<char*>(PageHead) + m_oaStats.PageSize_)) ? false : IsOutside;
			ActualPage = (toFreeBlock >= PageHead && reinterpret_cast<char*>(toFreeBlock) < (reinterpret_cast<char*>(PageHead) + m_oaStats.PageSize_)) ? PageHead : ActualPage;
			PageHead = PageHead->Next;
		}

		if (IsOutside)
			throw GE::Debug::Exception<ObjectAllocator>(GE::Debug::LEVEL_ERROR, ErrMsg("Freeing a block outside the object allocator"));

		int diff = static_cast<int>((reinterpret_cast<char*>(toFreeBlock) - m_oaConfig.PadBytes_ ) - (reinterpret_cast<char*>(ActualPage) + m_oaConfig.LeftAlignSize_ + sizeof(GenericObject)));
		int modVal = static_cast<int>(diff % m_blockSize);
		if (modVal)
			throw GE::Debug::Exception<ObjectAllocator>(GE::Debug::LEVEL_ERROR, ErrMsg("Freeing an unaligned block"));

		memset(toFreeBlock, 0, m_oaStats.ObjectSize_);
		if (FreeList_)
		{
			toFreeBlock->Next = FreeList_;
			FreeList_ = toFreeBlock;
		}
		else
		{
			FreeList_ = toFreeBlock;
			toFreeBlock->Next = nullptr;
		}

		//test = FreeList_;
		//std::cout << "\n(AFT) FREE FOR " << m_oaStats.ObjectSize_ << "\n";
		//while (test)
		//{
		//	std::cout << "Block: " << test << "-> " << test->Next << "\n";
		//	test = test->Next;
		//}
		//std::cout << "-------------------------------------------\n";

		m_oaStats.FreeObjects_++;
		m_oaStats.Deallocations_++;
		m_oaStats.ObjectsInUse_--;
		return;
	}



	char* useless = reinterpret_cast<char*>(Object);
	delete[] useless;
	m_oaStats.Deallocations_++;
	m_oaStats.ObjectsInUse_--;

}

// Frees all empty page
unsigned ObjectAllocator::FreeEmptyPages()
{

	GenericObject* freeHead{ FreeList_ };
	GenericObject* pageHead{ PageList_ };

	unsigned pagesFreed{ 0 };
	unsigned* pageStats = new unsigned[m_oaStats.PagesInUse_];
	GenericObject** allPages = new GenericObject * [m_oaStats.PagesInUse_];

	//initialize all the values first (to prevent warning)
	for (unsigned i{ 0 }; i < m_oaStats.PagesInUse_; ++i)
	{
		pageStats[i] = 0;
		allPages[i] = nullptr;
	}

	//Get the starting position of all the pages
	unsigned count{ 0 };
	while (pageHead)
	{
		if (count < m_oaStats.PagesInUse_)
			allPages[count] = pageHead;
		++count;
		pageHead = pageHead->Next;
	}

	//Loop through the FreeList to keep track of the number of free blocks of each page
	while (freeHead)
	{
		for (unsigned i{ 0 }; i < m_oaStats.PagesInUse_; ++i)
		{
			if (freeHead > allPages[i] && reinterpret_cast<char*>(freeHead) < reinterpret_cast<char*>(allPages[i]) + m_oaStats.PageSize_)
			{
				pageStats[i] = pageStats[i] + 1;
				break;
			}


		}
		freeHead = freeHead->Next;
	}

	//Now we loop through the Free List again, but this time we take all the blocks we want to keep and store it in a new list
	freeHead = FreeList_;
	GenericObject* newFreeList{ nullptr };
	GenericObject* newFreeHead{ nullptr };

	while (freeHead)
	{
		bool removed{ false };
		for (unsigned i{ 0 }; i < m_oaStats.PagesInUse_; ++i)
		{
			if (!(pageStats[i] == m_oaConfig.ObjectsPerPage_))
			{
				if (freeHead >= allPages[i] && reinterpret_cast<char*>(freeHead) < reinterpret_cast<char*>(allPages[i]) + m_oaStats.PageSize_)
				{
					GenericObject* copy = freeHead;
					freeHead = freeHead->Next;
					removed = true;
					if (!newFreeList)
						newFreeHead = newFreeList = copy;
					else
					{
						newFreeList->Next = copy;
						newFreeList = newFreeList->Next;
					}
					copy->Next = nullptr;
					break;
				}
			}

		}
		if (!removed && freeHead)
			freeHead = freeHead->Next;
	}

	int szb{ 0 };
	GenericObject* headb{ newFreeHead };
	while (headb)
	{
		++szb;
		headb = headb->Next;
	}

	//Now we remove the free and remove the unwanted pages
	pageHead = PageList_;
	GenericObject* newPageList{ nullptr };
	GenericObject* newPageListHead{ nullptr };
	count = 0;


	while (pageHead)
	{
		if (pageStats[count] == m_oaConfig.ObjectsPerPage_)
		{
			m_oaStats.FreeObjects_ -= m_oaConfig.ObjectsPerPage_;
			char* useless = reinterpret_cast<char*>(pageHead);
			pageHead = pageHead->Next;
			delete[] useless;
			++pagesFreed;
		}
		else
		{
			GenericObject* copy = pageHead;
			pageHead = pageHead->Next;
			copy->Next = nullptr;
			if (!newPageList)
				newPageListHead = newPageList = copy;
			else
			{
				newPageList->Next = copy;
				newPageList = newPageList->Next;
			}
		}
		++count;

	}


	FreeList_ = newFreeHead;
	PageList_ = newPageListHead;


	int sz{ 0 };
	GenericObject* head{ FreeList_ };
	while (head)
	{
		++sz;
		head = head->Next;
	}

	for (unsigned i{ 0 }; i < m_oaStats.PagesInUse_; ++i)
	{
		pageStats[i] = 0;
		allPages[i] = nullptr;
	}
	delete[] pageStats;
	delete[] allPages;
	m_oaStats.PagesInUse_ -= pagesFreed;
	return pagesFreed;
}


const void* ObjectAllocator::GetFreeList() const  // returns a pointer to the internal free list
{
	return FreeList_;
}
const void* ObjectAllocator::GetPageList() const  // returns a pointer to the internal page list
{
	return PageList_;
}
OAConfig ObjectAllocator::GetConfig() const       // returns the configuration parameters
{
	return m_oaConfig;
}
OAStats ObjectAllocator::GetStats() const        // returns the statistics for the allocator
{
	return m_oaStats;
}

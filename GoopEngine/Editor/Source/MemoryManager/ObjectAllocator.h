/*!
@file    ObjectAllocator.h
@author  han.q@digipen.edu
@date    01/30/2024

This file implements functionality useful and necessary to build a simple memory Manager.
Each Object Allocator allocates dynamic memory and store them as pages.
The free memories are keep tracked using the free list.
User will use the object allcoator to allocacte and free dynamic memory

*//*__________________________________________________________________________*/
//---------------------------------------------------------------------------
#ifndef OBJECTALLOCATORH
#define OBJECTALLOCATORH
//---------------------------------------------------------------------------
#define UNREFERENCED_PARAMETER(P)          (P)
#include <string>
#include <DebugTools/Exception/Exception.h>

// If the client doesn't specify these:
static const int DEFAULT_OBJECTS_PER_PAGE = 341;  
static const int DEFAULT_MAX_PAGES = 12;

//static const int DEFAULT_OBJECTS_PER_PAGE = 341;
//static const int DEFAULT_MAX_PAGES = 12;


/*!
  ObjectAllocator configuration parameters
*/
struct OAConfig
{
  /*!
    Constructor

    \param UseCPPMemManager
      Determines whether or not to by-pass the OA.

    \param ObjectsPerPage
      Number of objects for each page of memory.

    \param MaxPages
      Maximum number of pages before throwing an exception. A value
      of 0 means unlimited.

    \param DebugOn
      Is debugging code on or off?

    \param PadBytes
      The number of bytes to the left and right of a block to pad with.

    \param HBInfo
      Information about the header blocks used

    \param Alignment
      The number of bytes to align on.
  */
  OAConfig(bool UseCPPMemManager = false,
           unsigned ObjectsPerPage = DEFAULT_OBJECTS_PER_PAGE, 
           unsigned MaxPages = DEFAULT_MAX_PAGES, 
           bool DebugOn = false, 
           unsigned PadBytes = 0,
           unsigned Alignment = 0) : UseCPPMemManager_(UseCPPMemManager),
                                     ObjectsPerPage_(ObjectsPerPage), 
                                     MaxPages_(MaxPages), 
                                     DebugOn_(DebugOn), 
                                     PadBytes_(PadBytes),
                                     Alignment_(Alignment)
  {
    LeftAlignSize_ = 0;  
    InterAlignSize_ = 0;
  }

  bool UseCPPMemManager_ = false;                          //!< by-pass the functionality of the OA and use new/delete
  unsigned ObjectsPerPage_ = DEFAULT_OBJECTS_PER_PAGE;    //!< number of objects on each page
  unsigned MaxPages_ = DEFAULT_MAX_PAGES;                   //!< maximum number of pages the OA can allocate (0=unlimited)
  bool DebugOn_ = false;                                   //!< enable/disable debugging code (signatures, checks, etc.)
  unsigned PadBytes_ = 0;                                  //!< size of the left/right padding for each block
  unsigned Alignment_ = 8;                                   //!< address alignment of each block
  unsigned LeftAlignSize_ = 0;                            //!< number of alignment bytes required to align first block
  unsigned InterAlignSize_ = 0;                           //!< number of alignment bytes required between remaining blocks
};


/*!
  POD that holds the ObjectAllocator statistical info
*/
struct OAStats
{
  /*!
    Constructor
  */
  OAStats() : ObjectSize_(0), PageSize_(0), FreeObjects_(0), ObjectsInUse_(0), PagesInUse_(0),
                  MostObjects_(0), Allocations_(0), Deallocations_(0) {};

  size_t ObjectSize_;      //!< size of each object
  size_t PageSize_;        //!< size of a page including all headers, padding, etc.
  unsigned FreeObjects_;   //!< number of objects on the free list
  unsigned ObjectsInUse_;  //!< number of objects in use by client
  unsigned PagesInUse_;    //!< number of pages allocated
  unsigned MostObjects_;   //!< most objects in use by client at one time
  unsigned Allocations_;   //!< total requests to allocate memory
  unsigned Deallocations_; //!< total requests to free memory
};

/*!
  This allows us to easily treat raw objects as nodes in a linked list
*/
struct GenericObject
{
  GenericObject *Next; //!< The next object in the list
};

/*!
  This is used with external headers
*/
struct MemBlockInfo
{
  bool in_use;        //!< Is the block free or in use?
  char *label;        //!< A dynamically allocated NUL-terminated string
  unsigned alloc_num; //!< The allocation number (count) of this block
};

/*!
  This class represents a custom memory manager
*/

class ObjectAllocator
{
  public:
      // Creates the ObjectManager per the specified values
      // Throws an exception if the construction fails. (Memory allocation problem)
    ObjectAllocator(size_t ObjectSize);

      // Destroys the ObjectManager (never throws)
    ~ObjectAllocator();

      // Take an object from the free list and give it to the client (simulates new)
      // Throws an exception if the object can't be allocated. (Memory allocation problem)
    void *Allocate(const char *label = 0);

      // Returns an object to the free list for the client (simulates delete)
      // Throws an exception if the the object can't be freed. (Invalid object)
    void Free(void *Object);

      // Calls the callback fn for each block still in use
    unsigned DumpMemoryInUse() const;

      // Calls the callback fn for each block that is potentially corrupted
    unsigned ValidatePages() const;

      // Frees all empty page
    unsigned FreeEmptyPages();

    const void *GetFreeList() const;  // returns a pointer to the internal free list
    const void *GetPageList() const;  // returns a pointer to the internal page list
    OAConfig GetConfig() const;       // returns the configuration parameters
    OAStats GetStats() const;         // returns the statistics for the allocator

      // Prevent copy construction and assignment
    ObjectAllocator(const ObjectAllocator &oa) = delete;            //!< Do not implement!
    ObjectAllocator &operator=(const ObjectAllocator &oa) = delete; //!< Do not implement!

  private:
      // Some "suggested" members (only a suggestion!)
    GenericObject *PageList_; //!< the beginning of the list of pages
    GenericObject *FreeList_; //!< the beginning of the list of objects
    
    // Lots of other private stuff... 
    OAConfig m_oaConfig;
    OAStats m_oaStats;
    size_t m_blockSize;

    void AllocatePage();
};

#endif

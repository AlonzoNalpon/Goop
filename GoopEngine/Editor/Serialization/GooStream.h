/*!*********************************************************************
\file   GooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "rapidjson/document.h"
#include <string>

namespace GE
{
  namespace Serialization
  {
    // Type of data to be serialized
    /*enum class STREAM_MODE : uint8_t
    {
      IN,
      OUT
    };*/

    // Encapsulates raw data used for
    // serialization and deserialization
    template <typename Type>
    class GooStream
    {
    public:
      using size_type = size_t;
      using container_type = Type;

      GooStream(GooStream const&) = delete;
      virtual ~GooStream() {}

      // Implicit conversion to indicate status
      inline operator bool() const noexcept { return m_status; }

      // Returns the status of the stream based on the latest I/O operation
      inline bool Success() const noexcept { return m_status; }

      // Returns the number of elements currently in the stream
      inline size_type GetCount() const noexcept { return m_elementCount; }

      // Returns true if stream is empty
      inline bool IsEmpty() const noexcept { return m_elementCount == 0; }

      // Sets the status flag of the stream back to its default value
      inline void Clear() noexcept { m_status = true; }

      // Empties the stream of its contents and resets all values
      void Reset() noexcept;

    protected:
      GooStream() : m_data{}, m_elementCount{} {}

      rapidjson::Document m_data;
      size_type m_elementCount;
      bool m_status = true;
    };
    #include "GooStream.tpp"
    
    template <typename Type>
    class InGooStream : virtual public GooStream<Type>
    {
    public:
      virtual bool Read(std::string const& json) = 0;
      virtual bool Unload(Type& container) = 0;

    protected:
    };

    template <typename Type>
    class OutGooStream : virtual public GooStream<Type>
    {
    public:
      virtual bool Read(std::string const& json) = 0;
      virtual bool Unload(Type& container) = 0;

    protected:
    };
  }
}
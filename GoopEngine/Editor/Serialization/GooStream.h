/*!*********************************************************************
\file   GooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <pch.h>
#include <rapidjson/document.h>
#include <sstream>
#include <variant>
#ifdef _DEBUG
#include <iostream>
#endif

namespace GE
{
  namespace Serialization
  {
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

      // Sets the status flag of the stream back to its default value
      inline void Clear() noexcept { m_status = true; }

      // Returns the number of elements currently in the stream
      inline size_type GetCount() const noexcept { return m_elements; }

      // Returns true if stream is empty
      inline bool IsEmpty() const noexcept { return m_elements == 0; }

      // Empties the stream of its contents and resets all values
      void Reset() noexcept
      {
        m_elements = 0;
        if (std::get_if<rapidjson::Document>(&m_data))
        {
          std::get<rapidjson::Document>(m_data).Clear();
        }
        else
        {
          std::ostringstream().swap(std::get<std::ostringstream>(m_data));
        }
      }

    protected:
      GooStream(bool json)
      {
        if (json) m_data = rapidjson::Document(); else m_data = std::ostringstream();
        m_elements = 0;
      }

      // GooStreams are either json or raw text-based
      std::variant<rapidjson::Document, std::ostringstream> m_data;
      size_type m_elements;
      bool m_status = true;
    };
  }
}
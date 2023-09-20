#pragma once
/*!*********************************************************************
\file   Serializer.h
\author chengen.lau\@digipen.edu
\date   20-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <rapidjson/document.h>
#include <sstream>

namespace GE
{
  namespace Serialization
  {
    class JsonSerializer
    {
    public:
      using size_type = size_t;

      // Returns the number of elements currently in the stream
      inline size_type GetCount() const noexcept { return m_data.Size(); }

      // Returns true if stream is empty
      inline bool IsEmpty() const noexcept { return m_data.Empty(); }

      // Empties the stream of its contents and resets all values
      void Reset() noexcept { m_data.Clear(); }

    protected:
      rapidjson::Document m_data;
    };

    class TextSerializer
    {
    public:
      using size_type = size_t;

      // Returns the number of elements currently in the stream
      inline size_type GetCount() const noexcept { return m_elements; }

      // Returns true if stream is empty
      inline bool IsEmpty() const noexcept { return m_elements == 0; }

      // Empties the stream of its contents and resets all values
      void Reset() noexcept
      {
        std::ostringstream().swap(m_data);
        m_elements = 0;
      }

    protected:
      std::ostringstream m_data;
      size_type m_elements = 0;
    };
  }
}

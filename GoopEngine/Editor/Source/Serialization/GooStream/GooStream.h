/*!*********************************************************************
\file   GooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  Base GooStream class, which is a serialization stream class 
        for the Goop Engine that defines the functions and data
        members that will be inherited. It functions similar to
        std::fstream. Note that this base class cannot be
        initialized and is only inherited from by other GooStreams.

        USAGE:
          - Every GooStream will contain either a rapidjson::Document
            or std::ostringstream (through std::variant) to hold the 
            serialized data depending on whether the data being read
            or written is formatted in JSON

          - Bool implicit conversion operator is defined to
            return based on status of latest read/write operation

          - Typically, depending on whether serialization or 
            deserialization, or both are required, you would get
            access to 2 functions Read() and Unload(). Read will
            read data from a file/container while Unload outputs
            the data into a file/container.

          - Example usage:
              std::map<std::string, std::string> assets;
              AssetGooStream ags{ "assets.json" };
              if (!ags)
              {
                // Error
              }
              if (!ags.Unload(assets))
              {
                // Error
              }

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <sstream>
#include <variant>

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

      // Implicit conversion operator to indicate status
      inline operator bool() const noexcept { return m_status; }

      /*!*********************************************************************
      \brief
        Returns the status of the latest read/write operation
      \return
        True if the stream is in good state and false otherwise
      ************************************************************************/
      inline bool Success() const noexcept { return m_status; }

      /*!*********************************************************************
      \brief
        Sets the status flag of the stream back to its default value
      ************************************************************************/
      inline void Clear() noexcept { m_status = true; }

      /*!*********************************************************************
      \brief
        Returns the number of elements currently in the stream
      \return
        Number of elements
      ************************************************************************/
      inline size_type GetCount() const noexcept { return m_elements; }

      /*!*********************************************************************
      \brief
        Checks if the stream is empty
      \return
        True if the stream is empty and false otherwise
      ************************************************************************/
      inline bool IsEmpty() const noexcept { return m_elements == 0; }

      /*!*********************************************************************
      \brief
        Empties the stream of its contents and resets all values
      ************************************************************************/
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
      template <typename WrapperType>
      using writer_type = rapidjson::PrettyWriter<WrapperType>;

      // Constructor taking in bool where 'true' means that
      // the serialization type is in JSON
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
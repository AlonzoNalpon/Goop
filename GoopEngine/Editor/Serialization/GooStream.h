#pragma once
#include "rapidjson/document.h"
#include <string>
#include <map>

namespace GE
{
  namespace Serialization
  {
    // Type of data to be serialized
    /*enum class Stream_Type
    {
      ASSET,
      OBJECT,
      CONFIG
    };*/

    // Encapsulates raw data used for
    // serialization and deserialization
    class GooStream
    {
    public:
      using size_type = size_t;

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

    // GooStream for Assets
    class AssetGooStream : public GooStream
    {
    public:
      using size_type = size_t;
      using container_type = std::map<std::string, std::string>;

      AssetGooStream() : GooStream() {}
      // Reads json file into GooStream object
      AssetGooStream(std::string const& json);

      // Read from a json file. Stream is overwritten by new file
      bool Read(std::string const& json);

      // Unloads contents into a AssetGooStream::container_type object
      bool Unload(container_type& container);

    private:
      static std::string const ASSETS_DIR;
    };
  }
}
#pragma once
#include "../Singleton/Singleton.h"
#include "GooStream.h"

namespace GE
{
  namespace Serialization 
  {
    class Serializer : public Singleton<Serializer>
    {
    public:
      // Serializes from a GooStream into a file
      //AssetGooStream Serialize(std::string const& fileName) const override;

      // Deserializes a GooStream into an std::map of <std::string, std::string>
      bool Deserialize(std::string const& fileName, AssetGooStream::container_type& container) const;

    private:

    };
  }
}
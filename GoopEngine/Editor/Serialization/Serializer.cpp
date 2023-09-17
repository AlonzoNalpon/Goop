#include "Serializer.h"
#include <utility>  // for std::move
#ifdef _DEBUG
#include <iostream>
#endif

using namespace GE::Serialization;

//AssetGooStream Serializer::Serialize(std::string const& fileName) const
//{

//}

bool Serializer::Deserialize(std::string const& fileName, AssetGooStream::container_type& container) const
{
	AssetGooStream ags{ fileName };
	if (!ags) { return false; }

	return ags.Unload(container);
}
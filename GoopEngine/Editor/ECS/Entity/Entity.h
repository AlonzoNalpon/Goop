#pragma once

#include <bitset>

namespace GE
{
	namespace ECS
	{
		constexpr size_t MAX_COMPONENTS{ 32 };

		using Entity = unsigned int;
		using ComponentSignature = std::bitset<MAX_COMPONENTS>;
	}
}
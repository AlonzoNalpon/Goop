#pragma once

#include <bitset>

namespace GE
{
	namespace ECS
	{
		constexpr size_t MAX_COMPONENTS{ 32 };

		using Entity = unsigned int;
		// This is just a bitfield to track unique components
		// Functionally the same as ComponentType
		using ComponentSignature = std::bitset<MAX_COMPONENTS>;
	}
}

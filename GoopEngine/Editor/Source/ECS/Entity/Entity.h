/*!*********************************************************************
\file   Entity.h
\author w.chinkibryan\@digipen.edu
\date   9-September-2023
\brief  
  Basic defines for required for entities.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once

#include <bitset>
#include <limits>

namespace GE
{
	namespace ECS
	{
		constexpr size_t MAX_COMPONENTS{ 32 };
		constexpr unsigned int INVALID_ID{ std::numeric_limits<unsigned int>::max() };

		using Entity = unsigned int;
		// This is just a bitfield to track unique components
		// Functionally the same as ComponentType
		using ComponentSignature = std::bitset<MAX_COMPONENTS>;
	}
}

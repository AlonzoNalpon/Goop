/*!******************************************************************
\file   ComponentTypes.h 
\author w.chinkitbryam\@digipen.edu
\date   19 September 2023
\brief  
  Contains the an enum of names matching components that exist for
	the engine.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
********************************************************************/
#pragma once
#include <pch.h>

namespace GE
{
	namespace ECS
	{
		enum class COMPONENT_TYPES
		{
			TRANSFORM = 0,
			BOXCOLLIDER,
			VELOCITY,
			GRAVITY,
			SPRITE,
			SPRITEANIM,
			MODEL,
			TWEEN,
			COMPONENTS_TOTAL
		};

		// Registration of Components to ECS follows this map
		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::map<COMPONENT_TYPES, std::string> componentsToString
		{
			{ COMPONENT_TYPES::TRANSFORM, "Transform" },
			{ COMPONENT_TYPES::BOXCOLLIDER, "BoxCollider" },
			{ COMPONENT_TYPES::VELOCITY, "Velocity" },
			{ COMPONENT_TYPES::GRAVITY, "Gravity" },
			{ COMPONENT_TYPES::SPRITE, "Sprite" },
			{ COMPONENT_TYPES::SPRITEANIM, "SpriteAnim" },
			{ COMPONENT_TYPES::MODEL, "Model" },
			{ COMPONENT_TYPES::TWEEN, "Tween" }
		};

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::unordered_map<std::string, COMPONENT_TYPES> stringToComponents
		{
			{ "Transform", COMPONENT_TYPES::TRANSFORM },
			{ "BoxCollider", COMPONENT_TYPES::BOXCOLLIDER },
			{ "Velocity", COMPONENT_TYPES::VELOCITY },
			{ "Gravity", COMPONENT_TYPES::GRAVITY },
			{ "Sprite", COMPONENT_TYPES::SPRITE },
			{ "SpriteAnim", COMPONENT_TYPES::SPRITEANIM },
			{ "Model", COMPONENT_TYPES::MODEL },
			{ "Tween", COMPONENT_TYPES::TWEEN }
		};

		/*!*********************************************************************
		\brief
			$BRIEF
		\param
			ComponentSignature (component signature)
			COMPONENT_TYPES (COMPONENT_TYPE)
		\return
			bool, true if bit is set.
		************************************************************************/
		inline bool IsBitSet(ECS::ComponentSignature lhs, ECS::COMPONENT_TYPES rhs) noexcept
		{
			return lhs[static_cast<unsigned>(rhs)];
		}

		std::vector<std::string> ComponentSignatureToString(ComponentSignature sig);
	}
}

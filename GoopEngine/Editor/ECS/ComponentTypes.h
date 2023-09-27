/*!******************************************************************
\file   ComponentTypes.h 
\author w.chinkitbryam\@digipen.edu
\date   19 September 2023
\brief  
  Contains the an enum of names matching components that exist for
	the engine.
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
			VELOCITY,
			GRAVITY,
			BOXCOLLIDER,
			TWEEN,
			SPRITE,
			SPRITEANIM,
			MODEL,
			COMPONENTS_TOTAL
		};

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::unordered_map<COMPONENT_TYPES, std::string> componentsToString
		{
			{ COMPONENT_TYPES::TRANSFORM, "Transform" },
			{ COMPONENT_TYPES::BOXCOLLIDER, "BoxCollider" },
			{ COMPONENT_TYPES::VELOCITY, "Velocity" },
			{ COMPONENT_TYPES::GRAVITY, "Gravity" },
			{ COMPONENT_TYPES::SPRITE, "Sprite" },
			{ COMPONENT_TYPES::SPRITEANIM, "SpriteAnim" },
			{ COMPONENT_TYPES::MODEL, "Model" }
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
			{ "Model", COMPONENT_TYPES::MODEL }
		};

		inline unsigned componentsToUInt(std::string const& component)
		{
			return static_cast<unsigned>(stringToComponents.at(component));
		}
	}
}

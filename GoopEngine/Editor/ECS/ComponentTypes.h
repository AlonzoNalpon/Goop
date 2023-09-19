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
	namespace ESC
	{
		enum COMPONENT_TYPES
		{
			TRANSFORM = 0,
			AABB,
			VELOCITY,
			GRAVITY,
			SPRITE,
			MODEL,
			ANIMATION,
			COMPONENTS_TOTAL
		};

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::map<COMPONENT_TYPES, std::string> componentsToString
		{
			{ TRANSFORM, "Transform" },
			{ AABB, "AABB" },
			{ VELOCITY, "Velocity" },
			{ GRAVITY, "Gravity" },
			{ SPRITE, "Sprite" },
			{ MODEL, "Model" },
			{ ANIMATION, "Animation" },
		};

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::map<std::string, COMPONENT_TYPES> stringToComponents
		{
			{ "Transform", TRANSFORM },
			{ "AABB", AABB },
			{ "Velocity", VELOCITY },
			{ "Gravity", GRAVITY },
			{ "Sprite", SPRITE },
			{ "Model", MODEL },
			{ "Animation", ANIMATION },
		};
	}
}

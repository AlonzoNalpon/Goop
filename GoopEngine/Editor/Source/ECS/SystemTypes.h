/*!******************************************************************
\file   SystemTypes.h
\author loh.j@digipen.edu
\date   28 September 2023
\brief
	Enum of the systems available in the game engine for the object
	factory.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
********************************************************************/
#pragma once
#include <pch.h>

namespace GE::ECS
{
	// Enum of all the system types available for our engine.
	enum class SYSTEM_TYPES
	{
		COLLISION = 0, // Collision system type
		PHYSICS, // Physics system type
		RENDERING, // Rendering system type
		SPRITE_ANIM, // Sprite animation system type
		DRAGGABLE_OBJECT, // Draggable object system type
		PLAYER_CONTROLLER, // Player controller system type
		TOTAL_SYSTEMS // Total number of system types
	};

	constexpr size_t TOTAL_SYSTEMS{ static_cast<size_t>(SYSTEM_TYPES::TOTAL_SYSTEMS) };
	using SystemSignature = std::bitset <TOTAL_SYSTEMS>;

	// Const map so only interated through ranged for,
	// or use the .at(<key>) function
	const std::unordered_map<SYSTEM_TYPES, std::string> systemsToString
	{
		{ SYSTEM_TYPES::PHYSICS, "Physics" },
		{ SYSTEM_TYPES::COLLISION, "Collision" },
		{ SYSTEM_TYPES::DRAGGABLE_OBJECT, "DraggableObject" },
		{ SYSTEM_TYPES::PLAYER_CONTROLLER, "PlayerController" },
		{ SYSTEM_TYPES::RENDERING, "Rendering" },
		{ SYSTEM_TYPES::SPRITE_ANIM, "SpriteAnim" }
	};

	// Const map so only interated through ranged for,
	// or use the .at(<key>) function
	const std::unordered_map<std::string, SYSTEM_TYPES> stringToSystems
	{
		{ "Physics", SYSTEM_TYPES::PHYSICS },
		{ "Collision", SYSTEM_TYPES::COLLISION },
		{ "DraggableObject", SYSTEM_TYPES::DRAGGABLE_OBJECT },
		{ "PlayerController", SYSTEM_TYPES::PLAYER_CONTROLLER },
		{ "Rendering", SYSTEM_TYPES::RENDERING },
		{ "SpriteAnim", SYSTEM_TYPES::SPRITE_ANIM }
	};

	/*!*********************************************************************
	\brief
		inline function to compare if the bit is true
	\param
		SystemSignature (system signature)
		SYSTEM_TYPES (SYSTEM_TYPE)
	\return
		bool, true if bit is set.
	************************************************************************/
	inline bool IsBitSet(GE::ECS::SystemSignature lhs, ECS::SYSTEM_TYPES rhs) noexcept
	{
		return lhs[static_cast<unsigned>(rhs)];
	}
}

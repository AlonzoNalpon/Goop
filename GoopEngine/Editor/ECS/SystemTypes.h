#pragma once
#include <pch.h>

namespace GE
{
	namespace ECS
	{
		enum class SYSTEM_TYPES
		{
			PHYSICS = 0,
			COLLISION,
			DRAGGABLE_OBJECT,
			PLAYER_CONTROLLER,
			TOTAL_SYSTEMS
		};

		constexpr size_t TOTAL_SYSTEMS{ static_cast<size_t>(SYSTEM_TYPES::TOTAL_SYSTEMS) };
		using SystemSignature = std::bitset <TOTAL_SYSTEMS>;

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::map<SYSTEM_TYPES, std::string> systemsToString
		{
			{ SYSTEM_TYPES::PHYSICS, "Physics" },
			{ SYSTEM_TYPES::COLLISION, "Collision" },
			{ SYSTEM_TYPES::DRAGGABLE_OBJECT, "DraggableObject" },
			{ SYSTEM_TYPES::PLAYER_CONTROLLER, "PlayerController" },
		};

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::map<std::string, SYSTEM_TYPES> stringToSystems
		{
			{ "Physics", SYSTEM_TYPES::PHYSICS },
			{ "Collision", SYSTEM_TYPES::COLLISION },
			{ "DraggableObject", SYSTEM_TYPES::DRAGGABLE_OBJECT },
			{ "PlayerController", SYSTEM_TYPES::PLAYER_CONTROLLER }
		};
	}
}

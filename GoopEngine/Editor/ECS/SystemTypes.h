#pragma once
#include <pch.h>

namespace GE
{
	namespace ECS
	{
		enum class SYSTEM_TYPES
		{
			PHYSICS_SYSTEM = 0,
			COLLISION_SYSTEM,
			DRAGGABLE_OBJECT_SYSTEM,
			PLAYER_CONTROLLER_SYSTEM,
			TOTAL_SYSTEMS
		};

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::map<SYSTEM_TYPES, std::string> systemsToString
		{
			{ SYSTEM_TYPES::PHYSICS_SYSTEM, "PhysicsSystem" },
			{ SYSTEM_TYPES::COLLISION_SYSTEM, "CollisionSystem" },
			{ SYSTEM_TYPES::DRAGGABLE_OBJECT_SYSTEM, "DraggableObjectSystem" },
			{ SYSTEM_TYPES::PLAYER_CONTROLLER_SYSTEM, "PlayerControllerSystem" },
		};

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::map<std::string, SYSTEM_TYPES> stringToSystems
		{
			{ "PhysicsSystem", SYSTEM_TYPES::PHYSICS_SYSTEM },
			{ "CollisionSystem", SYSTEM_TYPES::COLLISION_SYSTEM },
			{ "DraggableObjectSystem", SYSTEM_TYPES::DRAGGABLE_OBJECT_SYSTEM },
			{ "PlayerControllerSystem", SYSTEM_TYPES::PLAYER_CONTROLLER_SYSTEM }
		};
	}
}

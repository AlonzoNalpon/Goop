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
#include <unordered_map>
#include <bitset>

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
		POST_ROOT_TRANSFORM,
		PRE_ROOT_TRANSFORM,
		ENEMY_SYSTEM,
		TEXT_RENDER,
		TWEEN_SYSTEM,
		GAME_SYSTEMS,
		AUDIO_SYSTEM,
		TOTAL_SYSTEMS // Total number of system types
	};

	constexpr size_t TOTAL_SYSTEMS{ static_cast<size_t>(SYSTEM_TYPES::TOTAL_SYSTEMS) };
	using SystemSignature = std::bitset <TOTAL_SYSTEMS>;

	// Const map so only interated through ranged for,
	// or use the .at(<key>) function
	const std::unordered_map<SYSTEM_TYPES, std::string> systemsToString
	{
		{ SYSTEM_TYPES::PHYSICS, "PhysicsSystem" },
		{ SYSTEM_TYPES::COLLISION, "CollisionSystem" },
		{ SYSTEM_TYPES::DRAGGABLE_OBJECT, "DraggableObjectSystem" },
		{ SYSTEM_TYPES::PLAYER_CONTROLLER, "PlayerControllerSystem" },
		{ SYSTEM_TYPES::RENDERING, "RenderSystem" },
		{ SYSTEM_TYPES::SPRITE_ANIM, "SpriteAnimSystem" },
		{ SYSTEM_TYPES::ENEMY_SYSTEM, "EnemySystem" },
		{ SYSTEM_TYPES::PRE_ROOT_TRANSFORM, "PreRootTransformSystem" },
		{ SYSTEM_TYPES::POST_ROOT_TRANSFORM, "PostRootTransformSystem" },
		{ SYSTEM_TYPES::TEXT_RENDER, "TextRenderSystem" },
		{ SYSTEM_TYPES::TWEEN_SYSTEM, "TweenSystem" },
		{ SYSTEM_TYPES::GAME_SYSTEMS, "GameSystem" },
		{ SYSTEM_TYPES::AUDIO_SYSTEM, "AudioSystem" }
	};

	// Const map so only interated through ranged for,
	// or use the .at(<key>) function
	const std::unordered_map<std::string, SYSTEM_TYPES> stringToSystems
	{
		{ "PhysicsSystem", SYSTEM_TYPES::PHYSICS },
		{ "CollisionSystem", SYSTEM_TYPES::COLLISION },
		{ "DraggableObjectSystem", SYSTEM_TYPES::DRAGGABLE_OBJECT },
		{ "PlayerControllerSystem", SYSTEM_TYPES::PLAYER_CONTROLLER },
		{ "RenderSystem", SYSTEM_TYPES::RENDERING },
		{ "SpriteAnimSystem", SYSTEM_TYPES::SPRITE_ANIM },
		{ "EnemySystem", SYSTEM_TYPES::ENEMY_SYSTEM },
		{ "PreRootTransformSystem", SYSTEM_TYPES::PRE_ROOT_TRANSFORM },
		{ "PostRootTransformSystem", SYSTEM_TYPES::POST_ROOT_TRANSFORM },
		{ "TextRenderSystem", SYSTEM_TYPES::TEXT_RENDER },
		{ "TweenSystem", SYSTEM_TYPES::TWEEN_SYSTEM },
		{ "GameSystem", SYSTEM_TYPES::GAME_SYSTEMS },
		{ "AudioSystem", SYSTEM_TYPES::AUDIO_SYSTEM }
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

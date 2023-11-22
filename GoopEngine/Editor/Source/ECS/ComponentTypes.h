/*!*********************************************************************
\file   ComponentTypes.h 
\author w.chinkitbryam\@digipen.edu
\date   19 September 2023
\brief  
  Contains the an enum of names matching components that exist for
	the engine.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <map>
#include <string>
#include <ECS/EntityComponentSystem.h>

namespace GE
{
	namespace ECS
	{
		enum COMPONENT_TYPES
		{
			TRANSFORM = 0,
			BOX_COLLIDER,
			VELOCITY,
			SPRITE,
			SPRITE_ANIM,
			TWEEN,
			SCRIPTS,
			DRAGGABLE,
			ENEMY_AI,
			TEXT,
			GAME,
			AUDIO,
			ANCHOR,
			GE_BUTTON,
			CARD,
			COMPONENTS_TOTAL
		};

		COMPONENT_TYPES& operator++(COMPONENT_TYPES& rhs);

		// Registration of Components to ECS follows this map
		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::map<COMPONENT_TYPES, std::string> componentsToString
		{
			{ COMPONENT_TYPES::TRANSFORM, "Transform" },
			{ COMPONENT_TYPES::BOX_COLLIDER, "BoxCollider" },
			{ COMPONENT_TYPES::VELOCITY, "Velocity" },
			{ COMPONENT_TYPES::SPRITE, "Sprite" },
			{ COMPONENT_TYPES::SPRITE_ANIM, "SpriteAnim" },
			{ COMPONENT_TYPES::TWEEN, "Tween" },
			{ COMPONENT_TYPES::SCRIPTS, "Scripts" },
			{ COMPONENT_TYPES::DRAGGABLE, "Draggable" },
			{ COMPONENT_TYPES::ENEMY_AI, "EnemyAI" },
			{ COMPONENT_TYPES::TEXT, "Text" },
			{ COMPONENT_TYPES::GAME, "Game" },
			{ COMPONENT_TYPES::AUDIO, "Audio" },
			{ COMPONENT_TYPES::ANCHOR, "Anchor" },
			{ COMPONENT_TYPES::GE_BUTTON, "Button" },
			{ COMPONENT_TYPES::CARD, "Card" }
		};

		// Const map so only interated through ranged for,
		// or use the .at(<key>) function
		const std::unordered_map<std::string, COMPONENT_TYPES> stringToComponents
		{
			{ "Transform", COMPONENT_TYPES::TRANSFORM },
			{ "BoxCollider", COMPONENT_TYPES::BOX_COLLIDER },
			{ "Velocity", COMPONENT_TYPES::VELOCITY },
			{ "Sprite", COMPONENT_TYPES::SPRITE },
			{ "SpriteAnim", COMPONENT_TYPES::SPRITE_ANIM },
			{ "Tween", COMPONENT_TYPES::TWEEN },
			{ "Scripts", COMPONENT_TYPES::SCRIPTS },
			{ "Draggable", COMPONENT_TYPES::DRAGGABLE },
			{ "EnemyAI", COMPONENT_TYPES::ENEMY_AI },
			{ "Text", COMPONENT_TYPES::TEXT },
			{ "Audio", COMPONENT_TYPES::AUDIO },
			{ "Button", COMPONENT_TYPES::GE_BUTTON },
			{ "Game", COMPONENT_TYPES::GAME },
			{ "Anchor", COMPONENT_TYPES::ANCHOR },
			{ "Card", COMPONENT_TYPES::CARD }
		};

		/*!*********************************************************************
		\brief
			Checks if bit is set.

		\param lhs
			Signature of the component

		\param rhs
			Component type that you check against
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

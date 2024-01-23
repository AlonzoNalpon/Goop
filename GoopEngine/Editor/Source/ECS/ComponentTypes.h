/*!*********************************************************************
\file   ComponentTypes.h 
\author w.chinkitbryam\@digipen.edu
\date   19 September 2023
\brief  
  Contains the list of component types used for registering to the ECS
	and for serialization.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <vector>
#include <rttr/type.h>
#include <string>
#include <Component/Components.h>

namespace GE
{
	namespace ECS
	{
		std::vector<rttr::type> const componentTypes
		{
			rttr::type::get<Component::Transform>(),
			rttr::type::get<Component::BoxCollider>(),
			rttr::type::get<Component::Velocity>(),
			rttr::type::get<Component::Sprite>(),
			rttr::type::get<Component::SpriteAnim>(),
			rttr::type::get<Component::Tween>(),
			rttr::type::get<Component::Scripts>(),
			rttr::type::get<Component::Draggable>(),
			rttr::type::get<Component::EnemyAI>(),
			rttr::type::get<Component::Text>(),
			rttr::type::get<Component::Game>(),
			rttr::type::get<Component::Audio>(),
			rttr::type::get<Component::GE_Button>(),
			rttr::type::get<Component::Card>(),
			rttr::type::get<Component::CardHolder>(),
			rttr::type::get<Component::CardHolderElem>(),
			rttr::type::get<Component::Emitter>()
		};
		
		/*!*********************************************************************
			\brief
				Converts a component signature to a vector of strings, where each
				string corresponds to the name of the component
			\param sig
				The component signature
			\return
				An std::vector of the component types in string
			************************************************************************/
		std::vector<std::string> ComponentSignatureToString(ComponentSignature sig);
	}
}

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
			rttr::type::get<GE::Component::Transform>(),
			rttr::type::get<GE::Component::BoxCollider>(),
			rttr::type::get<GE::Component::Velocity>(),
			rttr::type::get<GE::Component::Sprite>(),
			rttr::type::get<GE::Component::SpriteAnim>(),
			rttr::type::get<GE::Component::Tween>(),
			rttr::type::get<GE::Component::Scripts>(),
			rttr::type::get<GE::Component::EnemyAI>(),
			rttr::type::get<GE::Component::Text>(),
			rttr::type::get<GE::Component::Audio>(),
			rttr::type::get<GE::Component::GE_Button>(),
			rttr::type::get<GE::Component::Emitter>(),
			rttr::type::get<GE::Component::AnimEvents>()
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

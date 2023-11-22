/*!*********************************************************************
\file   Buttons.h
\author loh.j\@digipen.edu
\date   21 November 2023
\brief

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Systems/Button/ButtonTypes.h>

namespace GE::Component
{
	struct GE_Button
	{
		GE::ECS::BUTTON_TYPES m_buttonType = GE::ECS::BUTTON_TYPES::CHANGE_SCENE;
		std::string m_nextScene = "";
	};
}
/*!*********************************************************************
\file   Buttons.h
\author loh.j\@digipen.edu
\date   21 November 2023
\brief
	Component for making buttons.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Systems/Button/ButtonTypes.h>

namespace GE::Component
{
	struct GE_Button
	{
		enum ButtonEventType
		{
			NO_EVENT,
			SELECT_CARD,
			UNSELECT_CARD,
			CHANGE_SCENE,
			UNPAUSE,
			NEXT_TURN,
			QUIT_GAME,
			POPUP,
			TOTAL_EVENTS,
		};

		ButtonEventType m_eventType{NO_EVENT};
		std::string m_param = "";
	};
}
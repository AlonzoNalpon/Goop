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
		enum ButtonEventType
		{
			NO_EVENT,
			SELECT_CARD,
			UNSELECT_CARD,
			CHANGE_SCENE,
			UNPAUSE,
			NEXT_TURN,
			TOTAL_EVENTS,
		};

		ButtonEventType m_eventType{NO_EVENT};
		std::string m_param = "";
	};
	//class Button // Goop Button
	//{
	//public:

	//	inline std::string GetNextScene() const { return m_param; }

	//private:
	//	std::string m_param = "";
	//	// Empty struct to indicate entity as draggable
	//};
}
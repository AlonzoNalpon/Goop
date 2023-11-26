#pragma once
#include <rttr/rttr_enable.h>
/*!*********************************************************************
\file   ICommand.h
\author han.q\@digipen.edu
\date   26 Novemeber 2023
\brief
	Contains declaration for parent/base class ICommand. 
	This class will be used to create different command classes for each action 
	we want to undo/redo


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
namespace GE
{
	namespace CMD
	{
		class ICommand
		{
			public:
				/*!*********************************************************************
				\brief
					Virtual Execute function, the base will not have any definition. But deriveed child classes will need to define it
				************************************************************************/
				void virtual Execute() = 0;

				/*!*********************************************************************
				\brief
					Virtual Undo function, the base will not have any definition. But deriveed child classes will need to define it
				************************************************************************/
				void virtual Undo() = 0;

				/*!*********************************************************************
				\brief
					Virtual Redo function, the base will not have any definition. But deriveed child classes will need to define it
				************************************************************************/
				void virtual Redo() = 0;

				RTTR_ENABLE();
		};

	}
}

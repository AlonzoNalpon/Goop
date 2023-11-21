/*!*********************************************************************
\file   ToolBar.h 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Wrapper class to create an EditorGUI for the tool bar

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef NO_IMGUI
namespace GE::EditorGUI
{
	class ToolBar
	{
	public:
		/*!******************************************************************
		\brief
			Wrapper to create tool bar ribbon
		********************************************************************/
		static void CreateContent();
	};
}

#endif
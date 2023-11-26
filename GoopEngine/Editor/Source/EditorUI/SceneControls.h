/*!**********************************************************************
\file   SceneControls.h 
\author w.chinkitbryam\@digipen.edu
\date   25 October 2023
\brief    
  Wrapper class to create an EditorGUI for scene controls

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
namespace GE::EditorGUI
{
	class SceneControls
	{
	public:
		/*!******************************************************************
		\brief 
		  Wrapper to create Pause/Play/Stop/Step content
		********************************************************************/
		static void CreateContent();
	};
}
#endif
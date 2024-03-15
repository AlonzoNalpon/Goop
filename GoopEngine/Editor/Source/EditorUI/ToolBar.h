/*!*********************************************************************
\file   ToolBar.h 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Wrapper class to create an EditorGUI for the tool bar

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
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

	private:

		static bool m_scenePopup, m_prefabPopup;

		/*!*********************************************************************
		\brief
			Runs the code for the popup when "New Scene" option has been
			selected from the toolbar
		************************************************************************/
		static void RunNewScenePopup();

		/*!*********************************************************************
		\brief
		  Runs the code for the popup when "New Prefab" option has been
			selected from the toolbar
		************************************************************************/
		static void RunNewPrefabPopup();
	};
}

#endif
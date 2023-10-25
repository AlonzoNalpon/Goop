/*!*********************************************************************
\file   SceneHeirachy.h 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Wrapper class to create EditorGUI for a scene heirachy of all the
	entities

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include "ImGuiUI.h"

namespace GE::EditorGUI
{
	class SceneHierachy
	{
	public:
		/*!******************************************************************
		\brief
			Wrapper to create Scene graph
		********************************************************************/
		static void CreateContent();
	};
}


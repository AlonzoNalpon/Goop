/*!*********************************************************************
\file   Inspector.h
\author w.chinkitbryam\@digipen.edu
\date   23 October 2023
\brief
	Wrapper class to create an EditorGUI for the tool bar

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Graphics/Renderer/FrameBufferInfo.h>
namespace GE::EditorGUI
{
	class Inspector
	{
	public:
		/*!******************************************************************
		\brief
			Wrapper to create Component inspector
		********************************************************************/
		static void CreateContent();
	};
}


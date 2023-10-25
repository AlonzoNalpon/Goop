/*!**********************************************************************
\file   SceneControls.h
\author w.chinkitbryam\@digipen.edu
\date   25 October 2023
\brief
  Wrapper class to create an EditorGUI for scene controls

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "SceneControls.h"

using namespace ImGui;

void GE::EditorGUI::SceneControls::CreateContent()
{
  if (ImGui::BeginMenuBar()) 
  {
    static bool play;
    if (play)
    {
      //if (Button("Stop"));
      Button("Stop");
    }
    else
    {
      //if (Button("Play"));
      Button("Play");
    }

    //if (Button("Pause"));
    //if (Button("Step"));
    Button("Pause");
    Button("Step");

    EndMenuBar();
  }
}

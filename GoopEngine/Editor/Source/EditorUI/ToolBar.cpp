/*!*********************************************************************
\file   ToolBar.cpp 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Defines tool bar behaviour and logic for the EditorGUI

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ToolBar.h"
#include <ImGui/imgui.h>
#include <AssetManager/AssetManager.h>
#include <Serialization/Serialization.h>
#include <EditorUI/DataViz/Visualizer.h>
#include <ImGui/imgui_internal.h>
#include <ObjectFactory/GivingMyself90MinToFigureThisOut.h>

using namespace ImGui;
using namespace GE::EditorGUI::DataViz;

void GE::EditorGUI::ToolBar::CreateContent()
{
  if (BeginMenuBar())
  {
    if (BeginMenu("File"))
    {
      if (Selectable("New Scene"))
      {
        // Creates a new empty scene JSON
        // Loads it
      }

      if (Selectable("Save"))
      {
        // Save systems back to original file
        //Serialization::SerializeSystems(*Assets::AssetManager::GetInstance().GetConfigData<std::string>("Systems"));

        Serialization::SerializeScene("Assets/Scenes/Serialized.scn");
      }
      if (Selectable("Open"))
      {
        // Load scene function
      }

      ImGui::EndMenu();
    }
    EndMenuBar();
  }

  if (BeginMenuBar())
  {
    if (BeginMenu("View"))
    {
      if (ImGui::MenuItem("Performance Visualizer", nullptr, Visualizer::IsPerformanceShown()))
      {
        Visualizer::TogglePerformanceTab();
      }
      /* Guess we dont need this for now?
      if (ImGui::MenuItem("Memory Monitor", nullptr, Visualizer::IsMemoryShown()))
      {
        Visualizer::ToggleMemoryTab();
      }*/
      ImGui::EndMenu();
    }

    if (BeginMenu("Options"))
    {
      ImGui::EndMenu();
    }

    EndMenuBar();
  }
}

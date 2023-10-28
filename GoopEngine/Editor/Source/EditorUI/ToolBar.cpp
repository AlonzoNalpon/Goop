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
#include <GameStateManager/GameStateManager.h>
#include <EditorUI/DataViz/Visualizer.h>
#include <ImGui/imgui_internal.h>
#include <EditorUI/AssetBrowser.h>

using namespace ImGui;
using namespace GE::EditorGUI::DataViz;

void GE::EditorGUI::ToolBar::CreateContent()
{
  if (BeginMainMenuBar())
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
        GSM::GameStateManager::GetInstance().SaveScene();
      }
      if (Selectable("Open"))
      {
        // Load scene function
        std::string const scenePath{ GE::EditorGUI::AssetBrowser::OpenFileExplorer("Scenes (*.scn)\0*.scn\0", 1) };

        if (!scenePath.empty())
        {
          // Extract filename from filepath before calling SetNextScene
          std::string::size_type const startPos{ scenePath.find_last_of("\\") + 1 };
          GSM::GameStateManager::GetInstance().SetNextScene(scenePath.substr(startPos, scenePath.find_last_of(".") - startPos));
        }
      }

      ImGui::EndMenu();
    }

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

    EndMainMenuBar();
  }
}

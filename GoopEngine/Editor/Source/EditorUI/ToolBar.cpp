/*!*********************************************************************
\file   ToolBar.cpp 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Defines tool bar behaviour and logic for the EditorGUI

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include "ToolBar.h"
#include <ImGui/imgui.h>
#include <AssetManager/AssetManager.h>
#include <GameStateManager/GameStateManager.h>
#include <EditorUI/DataViz/Visualizer.h>
#include <ImGui/imgui_internal.h>
#include <EditorUI/AssetBrowser.h>
#include "PrefabEditor.h"

using namespace ImGui;
using namespace GE::EditorGUI::DataViz;

void GE::EditorGUI::ToolBar::CreateContent()
{
  if (BeginMainMenuBar())
  {
    if (BeginMenu("File"))
    {
      const char* const sceneFilter{ "Scenes (*.scn)\0*.scn" }, *const initialDir{"./Assets/Scenes"};

      ImGui::BeginDisabled(PrefabEditor::IsEditingPrefab());
      if (Selectable("New Scene"))
      {
        std::string const newScenePath{ GE::EditorGUI::AssetBrowser::SaveFileToExplorer(sceneFilter, 1, initialDir) };
        
        if (!newScenePath.empty())
        {
          GE::GSM::GameStateManager::GetInstance().LoadSceneFromExplorer(newScenePath);
        }
      }

      if (Selectable("Save"))
      {
        GSM::GameStateManager::GetInstance().SaveScene();
      }
      if (Selectable("Open"))
      {
        // Load scene function
        std::string const scenePath{ GE::EditorGUI::AssetBrowser::LoadFileFromExplorer(sceneFilter, 1, initialDir)};

        if (!scenePath.empty())
        {
          GE::GSM::GameStateManager::GetInstance().LoadSceneFromExplorer(scenePath);
        }
      }
      ImGui::EndDisabled();

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
#endif
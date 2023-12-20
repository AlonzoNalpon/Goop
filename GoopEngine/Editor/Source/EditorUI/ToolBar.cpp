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
#include "../ImGui/misc/cpp/imgui_stdlib.h"
#include <AssetManager/AssetManager.h>
#include <GameStateManager/GameStateManager.h>
#include <EditorUI/DataViz/Visualizer.h>
#include <ImGui/imgui_internal.h>
#include <EditorUI/AssetBrowser.h>
#include "PrefabEditor.h"
#include <Events/EventManager.h>

using namespace ImGui;
using namespace GE::EditorGUI::DataViz;

bool GE::EditorGUI::ToolBar::m_prefabPopup{ false };

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

      if (Selectable("New Prefab"))
      {
        m_prefabPopup = true;
      }

      if (Selectable("Save Scene"))
      {
        GSM::GameStateManager::GetInstance().SaveScene();
      }
      if (Selectable("Load Scene"))
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

    if (m_prefabPopup)
    {
      ImGui::OpenPopup("Create New Prefab");
      m_prefabPopup = false;
    }
    RunNewPrefabPopup();

    EndMainMenuBar();
  }
}

void GE::EditorGUI::ToolBar::RunNewPrefabPopup()
{
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Create New Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    static std::string prefabName{};
    static bool displayWarningText{ false };

    if (displayWarningText)
    {
      //ImVec4(0.8f, 0.2f, 0.2f, 1.f)
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name cannot be blank!!!");
    }

    ImGui::Text("Name of Prefab:");
    ImGui::SameLine();
    if (ImGui::InputText("##PrefabNameInput", &prefabName))
    {
      displayWarningText = false;
    }

    ImGui::SetCursorPosX(0.5f * (ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Cancel Create ").x));
    if (ImGui::Button("Cancel"))
    {
      prefabName.clear();
      displayWarningText = false;
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Create"))
    {
      // if name is blank / whitespace, reject it
      if (prefabName.find_first_not_of(" ") == std::string::npos)
      {
        displayWarningText = true;
      }
      else
      {
        Events::EventManager::GetInstance().Dispatch(Events::EditPrefabEvent(prefabName, {}));
        displayWarningText = false;
        prefabName.clear();
        ImGui::CloseCurrentPopup();
      }
    }

    ImGui::EndPopup();
  }
}
#endif
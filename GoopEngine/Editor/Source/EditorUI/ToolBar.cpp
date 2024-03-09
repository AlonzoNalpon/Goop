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
#include <Prefabs/PrefabManager.h>
#include <EditorUI/SpriteSheetEditor.h>

using namespace ImGui;
using namespace GE::EditorGUI::DataViz;

bool GE::EditorGUI::ToolBar::m_scenePopup{ false }, GE::EditorGUI::ToolBar::m_prefabPopup{ false };

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
        m_scenePopup = true;
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
      if (ImGui::MenuItem("Configure Sprite Sheets", nullptr, SpriteSheetEditor::IsToggled()))
      {
        SpriteSheetEditor::ToggleSpriteSheetEditor();
      }
      ImGui::EndMenu();
    }

    if (m_scenePopup)
    {
      ImGui::OpenPopup("Create New Scene");
      m_scenePopup = false;
    }
    else if (m_prefabPopup)
    {
      ImGui::OpenPopup("Create New Prefab");
      m_prefabPopup = false;
    }
    RunNewScenePopup();
    RunNewPrefabPopup();

    EndMainMenuBar();
  }
}

void GE::EditorGUI::ToolBar::RunNewScenePopup()
{
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Create New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    static std::string sceneName{};
    static bool blankWarning{ false }, existingSceneWarning{ false };

    if (blankWarning)
    {
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name cannot be blank!!!");
    }
    else if (existingSceneWarning)
    {
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Scene already exists!");
    }

    ImGui::Text("Name of Scene:");
    ImGui::SameLine();
    if (ImGui::InputText(".scn", &sceneName))
    {
      blankWarning = existingSceneWarning = false;
    }

    ImGui::SetCursorPosX(0.5f * (ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Cancel Create ").x));
    if (ImGui::Button("Cancel"))
    {
      sceneName.clear();
      blankWarning = existingSceneWarning = false;
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Create"))
    {
      // if name is blank / whitespace, reject it
      if (sceneName.find_first_not_of(" ") == std::string::npos)
      {
        blankWarning = true;
        existingSceneWarning = false;
      }
      else if (Assets::AssetManager::GetInstance().HasScene(sceneName))
      {
        existingSceneWarning = true;
        blankWarning = false;
      }
      else
      {
        Events::EventManager::GetInstance().Dispatch(Events::NewSceneEvent(sceneName));
        blankWarning = existingSceneWarning = false;
        sceneName.clear();
        ImGui::CloseCurrentPopup();
      }
    }

    ImGui::EndPopup();
  }
}

void GE::EditorGUI::ToolBar::RunNewPrefabPopup()
{
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Create New Prefab", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    static std::string prefabName{};
    static bool blankWarning{ false }, existingPrefabWarning{ false };

    if (blankWarning)
    {
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name cannot be blank!!!");
    }
    else if (existingPrefabWarning)
    {
      ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Prefab already exists!");
    }

    ImGui::Text("Name of Prefab:");
    ImGui::SameLine();
    if (ImGui::InputText("##PrefabNameInput", &prefabName))
    {
      blankWarning = existingPrefabWarning = false;
    }

    ImGui::SetCursorPosX(0.5f * (ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Cancel Create ").x));
    if (ImGui::Button("Cancel"))
    {
      prefabName.clear();
      blankWarning = existingPrefabWarning = false;
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Create"))
    {
      // if name is blank / whitespace, reject it
      if (prefabName.find_first_not_of(" ") == std::string::npos)
      {
        blankWarning = true;
        existingPrefabWarning = false;
      }
      else if (Prefabs::PrefabManager::GetInstance().DoesPrefabExist(prefabName))
      {
        existingPrefabWarning = true;
        blankWarning = false;
      }
      else
      {
        Events::EventManager::GetInstance().Dispatch(Events::EditPrefabEvent(prefabName, {}));
        blankWarning = existingPrefabWarning = false;
        prefabName.clear();
        ImGui::CloseCurrentPopup();
      }
    }

    ImGui::EndPopup();
  }
}
#endif
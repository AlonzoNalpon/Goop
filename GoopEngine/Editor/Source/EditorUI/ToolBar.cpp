/*!******************************************************************
\file   ToolBar.cpp 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Defines tool bar behaviour and logic for the EditorGUI
********************************************************************/
#include <pch.h>
#include "ToolBar.h"
#include <ImGui/imgui.h>
#include <AssetManager/AssetManager.h>
#include <Serialization/Serialization.h>
#include <EditorUI/DataViz/Visualizer.h>

using namespace ImGui;
using namespace GE::EditorGUI::DataViz;

void GE::EditorGUI::ToolBar::CreateContent()
{
  if (BeginMenuBar())
  {
    if (BeginMenu("File"))
    {
      // Sets the current context style to be invisible buttons
      ImGuiStyle& style = GetStyle();
      style.Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 0);
      style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0);
      style.Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 0);

      if (Button("New Scene"))
      {
        // Creates a new empty scene JSON
        // Loads it
      }

      if (Button("Save"))
      {
        // Save systems back to original file
        Serialization::SerializeSystems(*Assets::AssetManager::GetInstance().GetConfigData<std::string>("Systems"));
      }
      if (Button("Open"))
      {
        // Load scene function
      }

      // Reset the style to the default
      style = ImGuiStyle();

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
      if (ImGui::MenuItem("Memory Monitor", nullptr, Visualizer::IsMemoryShown()))
      {
        Visualizer::ToggleMemoryTab();
      }
      ImGui::EndMenu();
    }

    EndMenuBar();
  }

  if (BeginMenuBar())
  {
    if (BeginMenu("Options"))
    {
      ImGui::EndMenu();
    }

    EndMenuBar();
  }
}

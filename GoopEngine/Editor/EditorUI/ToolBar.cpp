/*!******************************************************************
\file   ToolBar.cpp 
\author w.chinkitbryam\@digipen.edu
\date   13 October 2023
\brief  
  Defines tool bar behaviour and logic for the EditorGUI
********************************************************************/

#include "ToolBar.h"
#include <ImGui/imgui.h>

using namespace ImGui;

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
        // Save scene function
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
    if (BeginMenu("Options"))
    {
      ImGui::EndMenu();
    }

    EndMenuBar();
  }
}

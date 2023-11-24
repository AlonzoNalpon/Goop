#include <pch.h>
#ifndef NO_IMGUI

#include <EditorUI/EditorTheme.h>
#include <ImGui/imgui.h>
void GE::EditorGUI::EditorTheme::SetImGuiTheme()
{
  // Get the default ImGui style
  ImGuiStyle& style = ImGui::GetStyle();

  // Modify the colors in the ImGui style
  ImVec4* colors = style.Colors;

  colors[ImGuiCol_WindowBg] = ImVec4(0.016f, 0.2f, 0.f, 1.0f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.055f, 0.569f, 0.f, 1.0f);

  // Example: Set the text color to a dark gray
  colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.0f);

  // Example: Set the button color
  //colors[ImGuiCol_Button] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  //colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
  //colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
}

#endif

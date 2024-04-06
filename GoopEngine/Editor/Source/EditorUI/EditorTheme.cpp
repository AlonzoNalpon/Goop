/*!*********************************************************************
\file   EditorTheme.cpp
\author a.nalpon\@digipen.edu
\date   27-November-2023
\brief  Editor style class. I am not claiming this code as it really
        isn't something i'd consider to be real work
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE

#include <EditorUI/EditorTheme.h>
#include <ImGui/imgui.h>


#include <InputManager/InputManager.h>

void GE::EditorGUI::EditorTheme::SetImGuiTheme()
{

  ImVec4* colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text] =                 ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_ChildBg] =              ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
  colors[ImGuiCol_TextDisabled] =         ImVec4(0.00f, 1.00f, 0.00f, 0.57f);
  colors[ImGuiCol_PopupBg] =              ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_FrameBgActive] =        ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
  colors[ImGuiCol_Border] =               ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
  colors[ImGuiCol_WindowBg] =             ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_FrameBg] =              ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] =       ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
  colors[ImGuiCol_BorderShadow] =         ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_MenuBarBg] =            ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
  colors[ImGuiCol_TitleBgActive] =        ImVec4(0.00f, 1.00f, 0.00f, 0.78f);
  colors[ImGuiCol_TitleBg] =              ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] =          ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
  colors[ImGuiCol_TitleBgCollapsed] =     ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
  colors[ImGuiCol_ScrollbarGrab] =        ImVec4(1.00f, 1.00f, 1.00f, 0.57f);
  colors[ImGuiCol_CheckMark] =            ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 0.00f, 0.57f);
  colors[ImGuiCol_SliderGrabActive] =     ImVec4(1.00f, 1.00f, 1.00f, 0.57f);
  colors[ImGuiCol_ScrollbarGrabActive] =  ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
  colors[ImGuiCol_SliderGrab] =           ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
  colors[ImGuiCol_ButtonHovered] =        ImVec4(1.00f, 1.00f, 1.00f, 0.57f);
  colors[ImGuiCol_Button] =               ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
  colors[ImGuiCol_Header] =               ImVec4(0.00f, 1.00f, 0.00f, 0.78f);
  colors[ImGuiCol_ButtonActive] =         ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
  colors[ImGuiCol_HeaderActive] =         ImVec4(0.00f, 1.00f, 0.00f, 0.57f);
  colors[ImGuiCol_Separator] =            ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
  colors[ImGuiCol_HeaderHovered] =        ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
  colors[ImGuiCol_SeparatorActive] =      ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_SeparatorHovered] =     ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
  colors[ImGuiCol_ResizeGripHovered] =    ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
  colors[ImGuiCol_ResizeGrip] =           ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
  colors[ImGuiCol_ResizeGripActive] =     ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
  colors[ImGuiCol_Tab] =                  ImVec4(0.f, 0.6f, 0.f, 1.00f); // the tab when it's not active

  colors[ImGuiCol_TabHovered] =           ImVec4(0.f, 1.0f, 0.f, 0.80f); // the tab when it's hovered on
  colors[ImGuiCol_TabActive] =            ImVec4(0.f, 0.8f, 0.18f, 1.00f); // the tab when it's active

  colors[ImGuiCol_TabUnfocused] =         ImVec4(0.f, 0.6f, 0.10f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] =   ImVec4(0.15f, 0.5f, 0.15f, 1.00f);

}

#endif

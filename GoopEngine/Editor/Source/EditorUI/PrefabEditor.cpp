/*!*********************************************************************
\file   PrefabEditor.cpp
\author chengen.lau\@digipen.edu
\date   3-November-2023
\brief  Contains the class encapsulating functions for the prefab editor
        feature.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include "PrefabEditor.h"
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>
#include <Prefabs/PrefabManager.h>
#include <ImGui/imgui.h>
#include <filesystem>

using namespace GE::EditorGUI;

GE::ECS::Entity PrefabEditor::m_prefabInstance{ ECS::INVALID_ID };
std::string PrefabEditor::m_prefabName, PrefabEditor::m_prefabPath;
bool PrefabEditor::m_isEditing{ false }, PrefabEditor::m_escapeTriggered{ false };

void PrefabEditor::HandleEvent(Events::Event* event)
{
  switch (event->GetCategory())
  {
  case Events::EVENT_TYPE::EDIT_PREFAB:
  {
    Events::EditPrefabEvent* prefabEvent{ static_cast<Events::EditPrefabEvent*>(event) };

    m_prefabPath = prefabEvent->m_path;
    m_prefabName = prefabEvent->m_prefab;
    m_prefabInstance = Prefabs::PrefabManager::GetInstance().SpawnPrefab(m_prefabName, {});

    // set pos to center
    Component::Transform* trans{ ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Transform>(m_prefabInstance) };
    trans->m_worldPos = {};
    m_isEditing = true;
    break;
  }
  case Events::EVENT_TYPE::KEY_TRIGGERED:
  {
    if (!m_isEditing) { return; }

    if (static_cast<Events::KeyTriggeredEvent*>(event)->GetKey() == GPK_ESCAPE)
    {
      m_escapeTriggered = true;
    }
  }
  }
}

void PrefabEditor::RenderBackToScenePopup()
{
  if (m_escapeTriggered)
  {
    ImGui::OpenPopup("Return to Scene");
    m_escapeTriggered = false;
  }

  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Return to Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Save changes made to prefab?");
    ImGui::SetCursorPosX(0.5f * (ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize("Discard Changes Save ").x));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.f));
    if (ImGui::Button("Discard Changes"))
    {
      m_isEditing = false;
      Events::EventManager::GetInstance().Dispatch(Events::StopSceneEvent());
      
      ResetPrefabEditor();
      ImGui::CloseCurrentPopup();
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.65f, 0.f, 1.f));
    if (ImGui::Button("Save"))
    {
      Events::EventManager& em{ Events::EventManager::GetInstance() };

      // check if valid here

      Prefabs::PrefabManager::GetInstance().CreatePrefabFromEntity(m_prefabInstance, m_prefabName, m_prefabPath);
      em.Dispatch(Events::StopSceneEvent());
      
      ResetPrefabEditor();
      ImGui::CloseCurrentPopup();
    }
    ImGui::PopStyleColor();

    ImGui::EndPopup();
  }
}

void PrefabEditor::ResetPrefabEditor()
{
  m_prefabInstance = ECS::INVALID_ID;
  m_prefabName.clear();
  m_prefabPath.clear();
  m_isEditing = false;
}

#endif
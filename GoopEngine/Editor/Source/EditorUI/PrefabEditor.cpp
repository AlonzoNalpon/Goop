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

#ifdef _DEBUG
#define PREFAB_EDITOR_DEBUG
#endif

using namespace GE::EditorGUI;

GE::ECS::Entity PrefabEditor::m_prefabInstance{ ECS::INVALID_ID };
std::unordered_map<GE::ECS::Entity, GE::Prefabs::PrefabSubData::SubDataId> PrefabEditor::m_entityToSubData;
std::vector<GE::Prefabs::PrefabSubData::SubDataId> PrefabEditor::m_removedChildren;
std::vector<std::pair<GE::Prefabs::PrefabSubData::SubDataId, rttr::type>> PrefabEditor::m_removedComponents;
std::string PrefabEditor::m_prefabName, PrefabEditor::m_prefabPath;
bool PrefabEditor::m_newPrefab{ false }, PrefabEditor::m_isEditing{ false }, PrefabEditor::m_escapeTriggered{ false };


void PrefabEditor::HandleEvent(Events::Event* event)
{
  switch (event->GetCategory())
  {
  case Events::EVENT_TYPE::EDIT_PREFAB:
  {
    Events::EditPrefabEvent* prefabEvent{ static_cast<Events::EditPrefabEvent*>(event) };

    m_prefabName = prefabEvent->m_prefab;
    m_newPrefab = prefabEvent->m_path.empty();
    if (m_newPrefab)
    {
      ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
      m_prefabInstance = ecs.CreateEntity();
      ecs.SetEntityName(m_prefabInstance, m_prefabName);
    }
    else
    {
      Prefabs::PrefabManager& pm{ Prefabs::PrefabManager::GetInstance() };
      m_prefabPath = prefabEvent->m_path;
      m_prefabInstance = pm.SpawnPrefab(m_prefabName, {});
      for (auto const& [objId, entityId] : pm.GetEntityPrefab(m_prefabInstance)->get().m_objToEntity)
      {
        m_entityToSubData.emplace(entityId, objId);
      }
    }

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

    break;
  }
  case Events::EVENT_TYPE::DELETE_PREFAB_CHILD:
  {
    auto iter{ m_entityToSubData.find(static_cast<Events::DeletePrefabChildEvent*>(event)->m_entity) };
    if (iter == m_entityToSubData.end())
    {
      Debug::ErrorLogger::GetInstance().LogError("Deletion of unknown Entity of ID "
        + std::to_string(static_cast<Events::DeletePrefabChildEvent*>(event)->m_entity));
      return;
    }
#ifdef PREFAB_EDITOR_DEBUG
    std::cout << "Child deletion detected: Added " << iter->second << " to m_removedChildren\n";
#endif

    m_removedChildren.emplace_back(iter->second);
    break;
  }
  case Events::EVENT_TYPE::DELETE_PREFAB_COMPONENT:
  {
    Events::DeletePrefabComponentEvent* deleteEvent{ static_cast<Events::DeletePrefabComponentEvent*>(event) };
    auto iter{ m_entityToSubData.find(deleteEvent->m_entity) };
    if (iter == m_entityToSubData.end())
    {
      std::ostringstream oss{};
      oss << "Deletion of " << deleteEvent->m_type.get_name().to_string()
          << " component for unknown Entity of ID " << deleteEvent->m_entity;
      Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return;
    }
#ifdef PREFAB_EDITOR_DEBUG
    std::cout << "Component deletion detected: Added <" << iter->second << ", " << deleteEvent->m_type << "> to m_removedComponents\n";
#endif

    m_removedComponents.emplace_back(std::make_pair(iter->second, deleteEvent->m_type));
    break;
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
      Prefabs::PrefabManager& pm{ Prefabs::PrefabManager::GetInstance() };

      if (m_removedChildren.empty() && m_removedComponents.empty())
      {
        pm.CreatePrefabFromEntity(m_prefabInstance, m_prefabName, m_prefabPath);
      }
      else
      {
#ifdef PREFAB_EDITOR_DEBUG
        std::cout << "Saving...\n";
#endif
        Prefabs::VariantPrefab prefab{ pm.CreateVariantPrefab(m_prefabInstance, m_prefabName) };
        Prefabs::PrefabVersion const& ver{ ++prefab.m_version };

        VerifyDeletedObjects(prefab);
        for (auto const& id : m_removedChildren)
        {
          prefab.m_removedChildren.emplace_back(id, ver);
        }
        for (auto const& [id, type] : m_removedComponents)
        {
          prefab.m_removedComponents.emplace_back(id, type, ver);
        }
        pm.UpdatePrefabFromEditor(m_prefabName, std::move(prefab));

      }
      em.Dispatch(Events::StopSceneEvent());
      
      ResetPrefabEditor();
      ImGui::CloseCurrentPopup();
    }
    ImGui::PopStyleColor();

    ImGui::EndPopup();
  }
}

void PrefabEditor::VerifyDeletedObjects(Prefabs::VariantPrefab const& prefab)
{
#ifdef PREFAB_EDITOR_DEBUG
  std::cout << "Verifying deleted objects...\n";
#endif
  // check through removed components for obsolete entries
  // e.g. if user removed a component and then added it back afterwards
  for (auto iter{ m_removedComponents.begin() }; iter != m_removedComponents.end();)
  {
    // if curr id is base entity
    if (iter->first == Prefabs::PrefabSubData::BasePrefabId)
    {
      rttr::type const& type{ iter->second };
      // if component exists in final prefab, remove entry
      if (std::find_if(prefab.m_components.cbegin(), prefab.m_components.cend(),
        [&type](rttr::variant const& comp) { return comp.get_type() == type; }) != prefab.m_components.cend())
      {
#ifdef PREFAB_EDITOR_DEBUG
        std::cout << "  Found " << type << " component in base object. Removing entry...\n";
#endif
        iter = m_removedComponents.erase(iter);
        continue;
      }
    }
    // if child was already removed, remove the entry
    else if (std::find(m_removedChildren.cbegin(), m_removedChildren.cend(), iter->first) != m_removedChildren.cend())
    {
#ifdef PREFAB_EDITOR_DEBUG
      std::cout << "  Found ID " << iter->first << " in deleted component entries. Removing entry since object is to be deleted...\n";
#endif
      iter = m_removedComponents.erase(iter);
      continue;
    }
    // if component exists in final prefab, remove entry
    auto const& [id, type] { *iter };
    auto obj{ std::find_if(prefab.m_objects.cbegin(), prefab.m_objects.cend(), [&id](Prefabs::PrefabSubData const& data) { return id == data.m_id; }) };
    if (obj == prefab.m_objects.cend())
    {
      Debug::ErrorLogger::GetInstance().LogError("Could not find child of prefab " + prefab.m_name + " with id " + std::to_string(id));
      iter = m_removedComponents.erase(iter);
      continue;
    }

    if (std::find_if(obj->m_components.cbegin(), obj->m_components.cend(),
      [&type](rttr::variant const& comp) { return comp.get_type() == type; }) != obj->m_components.cend())
    {
#ifdef PREFAB_EDITOR_DEBUG
      std::cout << "  Found " << type << " component in obj " << id << " . Removing entry...\n";
#endif
      iter = m_removedComponents.erase(iter);
      continue;
    }
  }
}

void PrefabEditor::ResetPrefabEditor()
{
  m_prefabInstance = ECS::INVALID_ID;
  m_prefabName.clear();
  m_prefabPath.clear();
  m_entityToSubData.clear();
  m_removedChildren.clear();
  m_removedComponents.clear();
  m_isEditing = m_newPrefab = false;
}

#endif
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
#include <ECS/ComponentTypes.h>

#ifdef _DEBUG
//#define PREFAB_EDITOR_DEBUG
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
      ecs.AddComponent(m_prefabInstance, Component::Transform({ 0, 0, 0 }, { 1, 1, 1 }, { 0, 0, 0 }));
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
  }
}

void PrefabEditor::RenderBackToScenePopup()
{
  if (!ImGui::IsPopupOpen("Return to Scene") && m_escapeTriggered)
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

      if (m_newPrefab)
      {
        pm.CreatePrefabFromEntity(m_prefabInstance, m_prefabName);
      }
      else
      {
        CheckForDeletions();
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
          Prefabs::PrefabVersion const ver{ pm.GetVariantPrefab(m_prefabName).m_version + 1 };

          for (auto const& id : m_removedChildren)
          {
            prefab.m_removedChildren.emplace_back(id, ver);
          }
          for (auto const& [id, type] : m_removedComponents)
          {
            prefab.m_removedComponents.emplace_back(id, type, ver);
          }
          pm.UpdatePrefabFromEditor(std::move(prefab), m_prefabPath);
        }
      }
      em.Dispatch(Events::StopSceneEvent());
      
      ResetPrefabEditor();
      ImGui::CloseCurrentPopup();
    }
    ImGui::PopStyleColor();

    ImGui::EndPopup();
  }
}

void PrefabEditor::CheckForDeletions()
{
  // if its a new prefab, ignore
  if (m_newPrefab) { return; }

#ifdef PREFAB_EDITOR_DEBUG
  std::cout << "Checking for deleted objects...\n";
#endif
  ObjectFactory::ObjectFactory const& of{ ObjectFactory::ObjectFactory::GetInstance() };
  Prefabs::PrefabManager& pm{ Prefabs::PrefabManager::GetInstance() };
  Prefabs::VariantPrefab const& ref{ pm.GetVariantPrefab(m_prefabName) };

  // check base entity first
  for (rttr::variant const& comp : ref.m_components)
  {
    rttr::type const compType{ comp.get_type().get_wrapped_type().get_raw_type() };
    auto iter{ ECS::stringToComponents.find(compType.get_name().to_string()) };
    if (iter == ECS::stringToComponents.end())
    {
      Debug::ErrorLogger::GetInstance().LogError("Unable to find " + compType.get_name().to_string() + " component type in ECS::stringToComponents");
      continue;
    }

    // if entity no longer contains component, means it was removed
    if (!of.GetEntityComponent(m_prefabInstance, iter->second).is_valid())
    {
#ifdef PREFAB_EDITOR_DEBUG
      std::cout << "  Added <0, " << compType << "> to m_removedComponents\n";
#endif
      m_removedComponents.emplace_back(Prefabs::PrefabSubData::BasePrefabId, compType);
    }
  }

  // now check its children and their components
  auto const mappings{ pm.GetEntityPrefab(m_prefabInstance)->get().m_objToEntity };
  for (auto const& subData : ref.m_objects)
  {
    // if child was removed, skip
    if (std::find(m_removedChildren.cbegin(), m_removedChildren.cend(), subData.m_id) != m_removedChildren.cend())
    {
#ifdef PREFAB_EDITOR_DEBUG
      std::cout << "  Skipping id " << subData.m_id << " since it was removed\n";
#endif
      continue;
    }

    // else continue to check components
    ECS::Entity const& currEntity{ mappings.at(subData.m_id) };
    for (rttr::variant const& comp : subData.m_components)
    {
      rttr::type const compType{ comp.get_type().get_wrapped_type().get_raw_type() };
      auto iter{ ECS::stringToComponents.find(compType.get_name().to_string()) };
      if (iter == ECS::stringToComponents.end())
      {
        Debug::ErrorLogger::GetInstance().LogError("Unable to find " + compType.get_name().to_string() + " component type in ECS::stringToComponents");
        continue;
      }

      // if entity no longer contains component, means it was removed
      if (!of.GetEntityComponent(currEntity, iter->second).is_valid())
      {
#ifdef PREFAB_EDITOR_DEBUG
        std::cout << "  Added <" << subData.m_id << ", " << compType << "> to m_removedComponents\n";
#endif
        m_removedComponents.emplace_back(subData.m_id, compType);
      }
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
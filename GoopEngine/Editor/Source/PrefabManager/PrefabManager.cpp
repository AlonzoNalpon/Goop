/*!*********************************************************************
\file   PrefabManager.cpp
\author chengen.lau\@digipen.edu
\date   27-November-2023
\brief  
  This file contains the definition of PrefabManager singleton.
  It is responsible for the mapping of entities to the prefabs they
  were created from. This is to allow each instance to be updated by
  any changes made to the prefab itself. The functions below are
  used to facilitate the adding and removing of entities to prefabs,
  as well as the updating of components based on prefabs.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include <PrefabManager/PrefabManager.h>
#include <ECS/EntityComponentSystem.h>
#include <ObjectFactory/ObjectFactory.h>
#include <Serialization/Serializer.h>

#ifdef _DEBUG
//#define PREFAB_MANAGER_DEBUG
#endif

using namespace GE::Prefabs;

void PrefabManager::AttachPrefab(ECS::Entity entity, EntityPrefabMap::mapped_type&& prefab)
{
  m_entitiesToPrefabs[entity] = prefab;
}

void PrefabManager::DetachPrefab(ECS::Entity entity)
{
  EntityPrefabMap::const_iterator entry{ m_entitiesToPrefabs.find(entity) };
  if (entry == m_entitiesToPrefabs.cend()) { return; }

  // remove entry if it exists
  m_entitiesToPrefabs.erase(entry);
}

bool PrefabManager::DoesEntityHavePrefab(ECS::Entity entity) const
{
  return m_entitiesToPrefabs.find(entity) != m_entitiesToPrefabs.cend();
}

PrefabManager::EntityPrefabMap::mapped_type const& PrefabManager::GetEntityPrefab(ECS::Entity entity) const
{
  EntityPrefabMap::const_iterator entry{ m_entitiesToPrefabs.find(entity) };
  if (entry == m_entitiesToPrefabs.cend())
  {
    std::ostringstream oss{}; oss << "Entity " << entity << " does not have a registered prefab";
    throw Debug::Exception<PrefabManager>(Debug::LEVEL_ERROR, ErrMsg(oss.str()));
  }

  return entry->second;
}

PrefabManager::PrefabVersion PrefabManager::GetPrefabVersion(std::string const& prefab) const
{
  std::unordered_map<std::string, PrefabVersion>::const_iterator entry{m_prefabVersions.find(prefab)};
  if (entry == m_prefabVersions.cend())
  {
    std::ostringstream oss{}; oss << "Prefab " << prefab << " does not exist";
    throw Debug::Exception<PrefabManager>(Debug::LEVEL_ERROR, ErrMsg(oss.str()));
  }

  return entry->second;
}

void PrefabManager::UpdateEntitiesFromPrefab(std::string const& prefab)
{
  //ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  ObjectFactory::ObjectFactory const& of{ ObjectFactory::ObjectFactory::GetInstance() };
  EntityPrefabMap::iterator iter{ m_entitiesToPrefabs.begin() };
  for (; iter != m_entitiesToPrefabs.cend(); ++iter)
  {
    EntityPrefabMap::mapped_type& iterVal{ iter->second };
    // if prefabs name don't match, continue
    if (iterVal.first != prefab) { continue; }

    // if prefab versions match, means its up-to-date so continue
    ObjectFactory::VariantPrefab const& prefabRef{ of.GetVariantPrefab(iterVal.first) };
    if (m_prefabVersions[prefab] == iterVal.second) { continue; }

    ObjectFactory::VariantPrefab prefabVar{ prefabRef };

    for (std::vector<rttr::variant>::const_iterator comp{ prefabVar.m_components.cbegin() }; comp != prefabVar.m_components.cend(); ++comp)
    {
      if (comp->get_type().get_wrapped_type() == rttr::type::get<Component::Transform*>())
      {
        Component::Transform const& newTrans{ *comp->get_value<Component::Transform*>() };
        Component::Transform* trans{ ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Transform>(iter->first) };
        auto pos{ std::move(trans->m_worldPos) };
        *trans = newTrans;
        trans->m_worldPos = std::move(pos);
        prefabVar.m_components.erase(comp);
        break;
      }
    }

    of.AddComponentsToEntity(iter->first, prefabVar.m_components);
    iterVal.second = m_prefabVersions[prefab];  // update version of entity
  }
}

void PrefabManager::UpdateAllEntitiesFromPrefab()
{
  for (auto const& [prefab, path] : Assets::AssetManager::GetInstance().GetPrefabs())
  {
    UpdateEntitiesFromPrefab(prefab);
  }
}

void PrefabManager::CreatePrefabFromEntity(ECS::Entity entity, std::string const& name) const
{
  ObjectFactory::VariantPrefab prefab{ name };
  for (unsigned i{}; i < static_cast<unsigned>(ECS::COMPONENT_TYPES::COMPONENTS_TOTAL); ++i)
  {
    rttr::variant comp{ Serialization::Serializer::GetEntityComponent(entity, static_cast<ECS::COMPONENT_TYPES>(i)) };
    if (!comp.is_valid()) { continue; }
    
    prefab.m_components.emplace_back(std::move(comp));
  }

  Assets::AssetManager& am{ Assets::AssetManager::GetInstance() };
  Serialization::Serializer::SerializeVariantToPrefab(prefab,
    am.GetConfigData<std::string>("Prefabs Dir") + name + am.GetConfigData<std::string>("Prefab File Extension"));
  am.ReloadFiles(Assets::FileType::PREFAB);

  GE::Debug::ErrorLogger::GetInstance().LogMessage(name + " saved to Prefabs");
}

void PrefabManager::HandleEvent(Events::Event* event)
{
  switch (event->GetCategory())
  {
  case Events::EVENT_TYPE::REMOVE_ENTITY:
  {
    EntityPrefabMap::const_iterator iter{ m_entitiesToPrefabs.find(static_cast<Events::RemoveEntityEvent*>(event)->m_entityId) };
    if (iter != m_entitiesToPrefabs.cend()) { m_entitiesToPrefabs.erase(iter); }
    break;
  }
  case Events::EVENT_TYPE::PREFAB_SAVED:
  {
    std::string const prefabName{ static_cast<Events::PrefabSavedEvent*>(event)->m_prefab };
    ++m_prefabVersions[prefabName];
    UpdateEntitiesFromPrefab(prefabName);
    GE::Debug::ErrorLogger::GetInstance().LogMessage("Entities in scene have been updated with changes to " + prefabName);
    break;
  }
  }
}

#endif

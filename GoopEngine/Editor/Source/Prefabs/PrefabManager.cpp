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
  Currently, the PrefabManager attaches a "version" to each prefab so
  that only outdated entities are updated with the prefab's components
  upon loading a scene.

  ** THIS CLASS ONLY RUNS IN THE EDITOR **
  
  Further improvements can be made so that an entity's component 
  should no longer be updated if it was changed externally through
  inspector.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#ifndef IMGUI_DISABLE
#include <Prefabs/PrefabManager.h>
#include <ECS/EntityComponentSystem.h>
#include <ObjectFactory/ObjectFactory.h>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>

#ifdef _DEBUG
//#define PREFAB_MANAGER_DEBUG
#endif

using namespace GE::Prefabs;

void PrefabManager::LoadPrefabsFromFile()
{
  auto const& prefabs{ GE::Assets::AssetManager::GetInstance().GetPrefabs() };
  for (auto const& [name, path] : prefabs)
  {
    m_prefabs.emplace(name, Serialization::Deserializer::DeserializePrefabToVariant(path));
  }
}

GE::ECS::Entity PrefabManager::SpawnPrefab(const std::string& key)
{
  PrefabDataContainer::const_iterator iter{ m_prefabs.find(key) };
  if (iter == m_prefabs.end())
  {
    throw GE::Debug::Exception<PrefabManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load prefab " + key));

    // Commented out for now - I think reloading should be responsibility of whoever calls this function
    //ReloadPrefabs();
    //if ((iter = m_prefabs.find(key)) == m_prefabs.end())
    //{
    //  throw GE::Debug::Exception<PrefabManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load prefab " + key));
    //}
  }

#ifdef PREFAB_V2
  auto mappedData{ iter->second.Construct() };
  ECS::Entity const newEntity{ mappedData.first };

  // set entity's prefab source
  m_entitiesToPrefabs[newEntity] = std::move(mappedData.second);
#else
  auto& ecs = ECS::EntityComponentSystem::GetInstance();
  ECS::Entity newEntity{ ecs.CreateEntity() };
  ObjectFactory::ObjectFactory::GetInstance().AddComponentsToEntity(newEntity, iter->second.m_components);

  // set entity's prefab source
  m_entitiesToPrefabs[newEntity] = { key, GetPrefabVersion(key) };

  ecs.SetEntityName(newEntity, key);
#endif

  return newEntity;
}

VariantPrefab const& PrefabManager::GetVariantPrefab(std::string const& name) const
{
  PrefabDataContainer::const_iterator ret{ m_prefabs.find(name) };
  if (ret == m_prefabs.cend()) { throw Debug::Exception<PrefabManager>(Debug::LEVEL_ERROR, ErrMsg("Unable to find prefab with name: " + name)); }

  return ret->second;
}

void PrefabManager::ReloadPrefab(std::string const& name)
{
  auto const& prefabs{ GE::Assets::AssetManager::GetInstance().GetPrefabs() };
  auto path{ GE::Assets::AssetManager::GetInstance().GetPrefabs().find(name) };

  if (path == prefabs.cend())
  {
    throw Debug::Exception<PrefabManager>(Debug::LEVEL_ERROR, ErrMsg("Unable to get path of prefab: " + name));
  }

  m_prefabs[name] = Serialization::Deserializer::DeserializePrefabToVariant(path->second);
}

void PrefabManager::ReloadPrefabs()
{
  m_prefabs.clear();
  LoadPrefabsFromFile();
}


void PrefabManager::AttachPrefab(ECS::Entity entity, EntityPrefabMap::mapped_type const& prefab)
{
#ifdef PREFAB_MANAGER_DEBUG
  std::cout << "Entity " << entity << ": " << prefab.m_prefab << ", version " << prefab.m_version << "\n";
#endif
  m_entitiesToPrefabs[entity] = prefab;
}
void PrefabManager::AttachPrefab(ECS::Entity entity, EntityPrefabMap::mapped_type&& prefab)
{
#ifdef PREFAB_MANAGER_DEBUG
  std::cout << "Entity " << entity << ": " << prefab.m_prefab << ", version " << prefab.m_version << "\n";
#endif
  m_entitiesToPrefabs[entity] = std::move(prefab);
}

void PrefabManager::DetachPrefab(ECS::Entity entity)
{
  EntityPrefabMap::const_iterator entry{ m_entitiesToPrefabs.find(entity) };
  if (entry == m_entitiesToPrefabs.cend()) { return; }

  // remove entry if it exists
  m_entitiesToPrefabs.erase(entry);
}

std::optional<PrefabManager::EntityPrefabMap::mapped_type> PrefabManager::GetEntityPrefab(ECS::Entity entity) const
{
  EntityPrefabMap::const_iterator entry{ m_entitiesToPrefabs.find(entity) };
  if (entry == m_entitiesToPrefabs.cend()) { return std::nullopt; }

  return entry->second;
}

VariantPrefab::PrefabVersion PrefabManager::GetPrefabVersion(std::string const& prefab)
{
  std::unordered_map<std::string, VariantPrefab::PrefabVersion>::const_iterator entry{ m_prefabVersions.find(prefab) };
  if (entry == m_prefabVersions.cend()) { return m_prefabVersions[prefab] = 0; }

  return entry->second;
}

void PrefabManager::UpdateEntitiesFromPrefab(std::string const& prefab)
{
  //ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  ObjectFactory::ObjectFactory const& of{ ObjectFactory::ObjectFactory::GetInstance() };
  EntityPrefabMap::iterator iter{ m_entitiesToPrefabs.begin() };
  rttr::type const transType{ rttr::type::get<Component::Transform*>() };

  for (; iter != m_entitiesToPrefabs.cend(); ++iter)
  {
    EntityPrefabMap::mapped_type& iterVal{ iter->second };
    // if prefabs name don't match, continue
    if (iterVal.m_prefab != prefab) { continue; }

    // if prefab versions match, means its up-to-date so continue
    if (m_prefabVersions[prefab] == iterVal.m_version)
    {
#ifdef PREFAB_MANAGER_DEBUG
      std::cout << " Entity " << iter->first << " matches " << prefab << "'s version of " << m_prefabVersions[prefab] << "\n";
#endif
      continue;
    }

    // for parent, update all components except worldPos in transform
    VariantPrefab const& prefabVar{ GetVariantPrefab(iterVal.m_prefab) };
    for (rttr::variant const& comp : prefabVar.m_components)
    {
      if (comp.get_type().get_wrapped_type() == transType)
      {
        Component::Transform& newTrans{ *comp.get_value<Component::Transform*>() };
        newTrans.m_worldPos = ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Transform>(iter->first)->m_worldPos;
        break;
      }
    }
    of.AddComponentsToEntity(iter->first, prefabVar.m_components);

    // update components of children but for transform, only update local pos, rot and scale
    auto const& mappedData{ iterVal.m_entityToObj };
    for (PrefabSubData const& obj : prefabVar.m_objects)
    {
      auto childEntity{ mappedData.find(obj.m_id) };
      if (childEntity == mappedData.cend())
      {
        std::ostringstream oss;
        oss << "Entity " << iter->first << " missing child object: " << obj.m_name << ". Skipping entity...";
        Debug::ErrorLogger::GetInstance().LogError(oss.str());
        continue;
      }

     /* for (rttr::variant const& comp : obj.m_components)
      {
        if (comp.get_type().get_wrapped_type() == transType)
        {
          Component::Transform& newTrans{ *comp.get_value<Component::Transform*>() };
          newTrans.m_worldPos = ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Transform>(iter->first)->m_worldPos;
          break;
        }
      }*/
      of.AddComponentsToEntity(childEntity->second, obj.m_components);
    }

    iterVal.m_version = m_prefabVersions[prefab];  // update version of entity
#ifdef PREFAB_MANAGER_DEBUG
    std::cout << "  Entity " << iter->first << " updated with " << prefab << " version " << iterVal.m_version << "\n";
#endif
  }
}

void PrefabManager::UpdateAllEntitiesFromPrefab()
{
  for (auto const& [prefab, path] : Assets::AssetManager::GetInstance().GetPrefabs())
  {
    UpdateEntitiesFromPrefab(prefab);
  }
}

#ifdef PREFAB_V2
void PrefabManager::CreatePrefabFromEntity(ECS::Entity entity, std::string const& name)
{
  VariantPrefab prefab{ name };
  prefab.m_components = ObjectFactory::ObjectFactory::GetInstance().GetEntityComponents(entity);

  prefab.CreateSubData(ECS::EntityComponentSystem::GetInstance().GetChildEntities(entity));

  Assets::AssetManager& am{ Assets::AssetManager::GetInstance() };
  Serialization::Serializer::SerializeVariantToPrefab(prefab,
    am.GetConfigData<std::string>("Prefabs Dir") + name + am.GetConfigData<std::string>("Prefab File Extension"));
  am.ReloadFiles(Assets::FileType::PREFAB);
  ReloadPrefabs();

  GE::Debug::ErrorLogger::GetInstance().LogMessage(name + " saved to Prefabs");
}
#else
void PrefabManager::CreatePrefabFromEntity(ECS::Entity entity, std::string const& name) const
{
  VariantPrefab prefab{ name };
  prefab.m_components = ObjectFactory::ObjectFactory::GetInstance().GetEntityComponents(entity);

  Assets::AssetManager& am{ Assets::AssetManager::GetInstance() };
  Serialization::Serializer::SerializeVariantToPrefab(prefab,
    am.GetConfigData<std::string>("Prefabs Dir") + name + am.GetConfigData<std::string>("Prefab File Extension"));
  am.ReloadFiles(Assets::FileType::PREFAB);
  ReloadPrefab(name);

  GE::Debug::ErrorLogger::GetInstance().LogMessage(name + " saved to Prefabs");
}
#endif


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
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
#include <Systems/RootTransform/PostRootTransformSystem.h>

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

GE::ECS::Entity PrefabManager::SpawnPrefab(const std::string& key, Math::dVec3 const& pos, bool mapEntity)
{
  PrefabDataContainer::const_iterator iter{ m_prefabs.find(key) };
  if (iter == m_prefabs.end())
  {
    throw GE::Debug::Exception<PrefabManager>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to load prefab " + key));
  }

  auto mappedData{ iter->second.Construct() };
  ECS::Entity const newEntity{ mappedData.first };
  GE::Component::Transform& trans{ *ECS::EntityComponentSystem::GetInstance().GetComponent<GE::Component::Transform>(newEntity)};
  trans.m_pos = pos;

  if (mapEntity)
  {
    // set entity's prefab source
    m_entitiesToPrefabs[newEntity] = std::move(mappedData.second);
  }

  Math::dMat4 identity
  {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
  };
  // Update recursively using entity's world transformation matrix
  Systems::PostRootTransformSystem::Propergate(newEntity, identity);

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

std::optional<std::reference_wrapper<PrefabManager::EntityPrefabMap::mapped_type>> PrefabManager::GetEntityPrefab(ECS::Entity entity)
{
  EntityPrefabMap::iterator entry{ m_entitiesToPrefabs.find(entity) };
  if (entry == m_entitiesToPrefabs.end()) { return std::nullopt; }

  return entry->second;
}

bool PrefabManager::UpdateEntitiesFromPrefab(std::string const& prefab)
{
  ObjectFactory::ObjectFactory const& of{ ObjectFactory::ObjectFactory::GetInstance() };
  rttr::type const transType{ rttr::type::get<Component::Transform*>() };
  bool instanceUpdated{ false };

  VariantPrefab const& prefabVar{ GetVariantPrefab(prefab) };
  for (EntityPrefabMap::iterator iter{ m_entitiesToPrefabs.begin() }; iter != m_entitiesToPrefabs.cend(); ++iter)
  {
    EntityPrefabMap::mapped_type& iterVal{ iter->second };
    ECS::Entity const& entity{ iter->first };

    // if prefabs name don't match or not registered, continue
    if (iterVal.m_prefab != prefab || !iterVal.m_registered) { continue; }

    // if prefab versions match, means its up-to-date so continue
    if (iterVal.m_version == prefabVar.m_version)
    {
#ifdef PREFAB_MANAGER_DEBUG
      std::cout << " Entity " << entity << " matches " << prefab << "'s version of " << prefabVar.m_version << "\n";
#endif
      continue;
    }

    instanceUpdated = true;
    ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
    // for parent, update all components except worldPos in transform
    auto pos{ ecs.GetComponent<Component::Transform>(entity)->m_pos };
    of.AddComponentsToEntity(entity, prefabVar.m_components);
    ecs.GetComponent<Component::Transform>(entity)->m_pos = std::move(pos);

    auto& mappedData{ iterVal.m_objToEntity };

    // apply each child removal that is later than current entity version
    for (auto childIter{ prefabVar.m_removedChildren.rbegin() };
      childIter != prefabVar.m_removedChildren.rend() && childIter->second > iterVal.m_version; ++childIter)
    {
      auto dataIter{ mappedData.find(childIter->first) };
      if (dataIter == mappedData.end())
      {
        std::ostringstream oss{};
        oss << "Unable to remove obj ID " << childIter->first << " when updating prefab instances of " << prefab;
        Debug::ErrorLogger::GetInstance().LogError(oss.str());
        continue;
      }
#ifdef PREFAB_MANAGER_DEBUG
      std::cout << "Updating child deletion: Removed entity " << dataIter->second << " (id: " << childIter->first << ")\n";
#endif
      ecs.DestroyEntity(dataIter->second);
      mappedData.erase(dataIter);
    }
    iterVal.Validate();

    // apply component removals that are later than current version
    for (auto compIter{ prefabVar.m_removedComponents.rbegin() };
      compIter != prefabVar.m_removedComponents.rend() && compIter->m_version > iterVal.m_version; ++compIter)
    {
      auto dataIter{ mappedData.find(compIter->m_id) };
      if (dataIter == mappedData.end())
      {
        std::ostringstream oss{};
        oss << "  Unable to remove " << compIter->m_type.get_name().to_string() << " component from obj with ID "
          << compIter->m_id << " when updating prefab instances of " << prefab;
        Debug::ErrorLogger::GetInstance().LogError(oss.str());
        continue;
      }

#ifdef PREFAB_MANAGER_DEBUG
      std::cout << "  Updating component deletion: Removed " << compIter->m_type 
        << " component from entity " << dataIter->second << " (id: " << compIter->m_id << ")\n";
#endif
      ObjectFactory::ObjectFactory::GetInstance().RemoveComponentFromEntity(dataIter->second, compIter->m_type);
    }

    bool newEntityCreated{ false };
    for (PrefabSubData const& obj : prefabVar.m_objects)
    {
      auto childEntity{ mappedData.find(obj.m_id) };
      if (childEntity == mappedData.cend())
      {
#ifdef PREFAB_MANAGER_DEBUG
        std::cout << "Entity " << iter->first << " missing child object: " << obj.m_name << ". Creating...\n";
#endif
        ECS::Entity const newChild{ obj.Construct() };
        mappedData.emplace(obj.m_id, newChild);
        newEntityCreated = true;
      }
      else
      {
        of.AddComponentsToEntity(childEntity->second, obj.m_components);
      }
    }

    // if new entity was created, iterate through all mappings
    // and re-establish hierarchy
    if (newEntityCreated)
    {
      for (PrefabSubData const& obj : prefabVar.m_objects)
      {
        ECS::Entity const child{ mappedData[obj.m_id] }, parent{ mappedData[obj.m_parent] };
        ecs.SetParentEntity(child, parent);
        ecs.AddChildEntity(parent, child);
      }
    }

    // set all child positions based on parent transform
    Math::dMat4 const identity
    {
      { 1, 0, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 1 }
    };
    Systems::PostRootTransformSystem::Propergate(entity, identity);

    iterVal.m_version = prefabVar.m_version;  // update version of entity
#ifdef PREFAB_MANAGER_DEBUG
    std::cout << "  Entity " << iter->first << " updated with " << prefab << " version " << iterVal.m_version << "\n";
#endif
  }

  return instanceUpdated;
}

bool PrefabManager::UpdateAllEntitiesFromPrefab()
{
  bool instanceUpdated{ false };
  for (auto const& [prefab, path] : Assets::AssetManager::GetInstance().GetPrefabs())
  {
    if (UpdateEntitiesFromPrefab(prefab)) { instanceUpdated = true; }
  }

  return instanceUpdated;
}

void PrefabManager::UpdatePrefabFromEditor(ECS::Entity prefabInstance, std::vector<Prefabs::PrefabSubData::SubDataId> const& removedChildren,
  std::vector<std::pair<Prefabs::PrefabSubData::SubDataId, rttr::type>> const& removedComponents, std::string const& filepath)
{
  PrefabDataContainer::iterator iter{ m_prefabs.find(ECS::EntityComponentSystem::GetInstance().GetEntityName(prefabInstance)) };
  if (iter == m_prefabs.end())
  {
    throw Debug::Exception<PrefabManager>(Debug::LEVEL_ERROR, ErrMsg("Trying to update non-existent prefab: "
      + ECS::EntityComponentSystem::GetInstance().GetEntityName(prefabInstance)));
  }
  VariantPrefab& original{ iter->second };
  VariantPrefab newPrefab{ CreateVariantPrefab(prefabInstance, original.m_name) };
  newPrefab.m_version = original.m_version + 1;

  // update prefab with removed objects
  for (auto const& elem : removedChildren)
  {
    original.m_removedChildren.emplace_back(elem, newPrefab.m_version);
  }
  for (auto const& [id, type] : removedComponents)
  {
    original.m_removedComponents.emplace_back(id, type, newPrefab.m_version);
  }
  newPrefab.m_removedChildren = std::move(original.m_removedChildren);
  newPrefab.m_removedComponents = std::move(original.m_removedComponents);

  Serialization::Serializer::SerializeVariantToPrefab(newPrefab, filepath);
  iter->second = std::move(newPrefab);
}

VariantPrefab PrefabManager::CreateVariantPrefab(ECS::Entity entity, std::string const& name)
{
  VariantPrefab prefab{ name };
  prefab.m_components = ObjectFactory::ObjectFactory::GetInstance().GetEntityComponents(entity);
  prefab.CreateSubData(ECS::EntityComponentSystem::GetInstance().GetChildEntities(entity));

  return prefab;
}

void PrefabManager::CreatePrefabFromEntity(ECS::Entity entity, std::string const& name, std::string const& path)
{
  // if prefab already exists, append "(Copy)" to it
  std::string prefabName{ name };
  while (DoesPrefabExist(prefabName))
  {
    prefabName += " (Copy)";
  }

  VariantPrefab prefab{ CreateVariantPrefab(entity, prefabName) };

  Assets::AssetManager& am{ Assets::AssetManager::GetInstance() };
  if (path.empty())
  {
    Serialization::Serializer::SerializeVariantToPrefab(prefab,
      am.GetConfigData<std::string>("Prefabs Dir") + prefabName + am.GetConfigData<std::string>("Prefab File Extension"));
  }
  else
  {
    Serialization::Serializer::SerializeVariantToPrefab(prefab, path);
  }

  am.ReloadFiles(Assets::AssetType::PREFAB);
  ReloadPrefab(prefabName);
  GE::Debug::ErrorLogger::GetInstance().LogMessage(prefabName + " saved to Prefabs");
}


void PrefabManager::HandleEvent(Events::Event* event)
{
  switch (event->GetCategory())
  {
  case Events::EVENT_TYPE::UNLOAD_SCENE:
  {
    m_entitiesToPrefabs.clear();
    break;
  }
  case Events::EVENT_TYPE::REMOVE_ENTITY:
  {
    EntityPrefabMap::const_iterator iter{ m_entitiesToPrefabs.find(static_cast<Events::RemoveEntityEvent*>(event)->m_entityId) };
    if (iter != m_entitiesToPrefabs.cend()) { m_entitiesToPrefabs.erase(iter); }
    break;
  }
  case Events::EVENT_TYPE::DELETE_PREFAB:
  {
    PrefabDataContainer::const_iterator iter{ m_prefabs.find(static_cast<Events::DeletePrefabEvent*>(event)->m_name) };
    if (iter != m_prefabs.cend()) { m_prefabs.erase(iter); }
    break;
  }
  }
}

#endif
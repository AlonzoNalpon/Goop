#include <pch.h>
#ifndef NO_IMGUI
#include <PrefabManager/PrefabManager.h>
#include <ECS/EntityComponentSystem.h>
#include <ObjectFactory/ObjectFactory.h>
#include <Serialization/Serializer.h>

using namespace GE::Prefabs;

void PrefabManager::AttachPrefab(ECS::Entity entity, std::string prefabName)
{
  m_entitiesToPrefabs[entity] = std::move(prefabName);
}

void PrefabManager::DetachPrefab(ECS::Entity entity)
{
  EntityPrefabMap::const_iterator entry{ m_entitiesToPrefabs.find(entity) };
  if (entry == m_entitiesToPrefabs.cend()) { return; }

  // remove entry if it exists
  m_entitiesToPrefabs.erase(entry);
}

std::string PrefabManager::GetEntityPrefab(ECS::Entity entity) const
{
  EntityPrefabMap::const_iterator entry{ m_entitiesToPrefabs.find(entity) };
  if (entry == m_entitiesToPrefabs.cend())
  {
    std::ostringstream oss{};
    oss << "Entity " << entity << " has no associated prefab";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }

  return entry->second;
}

void PrefabManager::UpdateEntitiesFromPrefab()
{
  //ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  ObjectFactory::ObjectFactory const& of{ ObjectFactory::ObjectFactory::GetInstance() };
  for (auto const& [entity, prefab] : m_entitiesToPrefabs)
  {
    ObjectFactory::VariantPrefab const& prefabVar{ of.GetVariantPrefab(prefab) };
    of.AddComponentsToEntity(entity, prefabVar.m_components);
  }

  GE::Debug::ErrorLogger::GetInstance().LogMessage("Entities in scene have been updated with prefab changes");
}

#endif

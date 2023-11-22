#include <pch.h>
#include <PrefabManager/PrefabManager.h>
#ifndef NO_IMGUI

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

#endif

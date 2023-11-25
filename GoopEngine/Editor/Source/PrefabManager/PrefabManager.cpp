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

void PrefabManager::UpdateEntitiesFromPrefab(std::string const& prefab)
{
  //ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
  ObjectFactory::ObjectFactory const& of{ ObjectFactory::ObjectFactory::GetInstance() };
  for (auto const& [entity, name] : m_entitiesToPrefabs)
  {
    if (name != prefab) { continue; }

    ObjectFactory::VariantPrefab const& prefabVar{ of.GetVariantPrefab(name) };
    for (rttr::variant const& comp : prefabVar.m_components)
    {
      if (comp.get_type().get_wrapped_type() == rttr::type::get<Component::Transform*>())
      {
        Component::Transform const& newTrans{ *comp.get_value<Component::Transform*>() };
        Component::Transform* trans{ ECS::EntityComponentSystem::GetInstance().GetComponent<Component::Transform>(entity) };
        auto pos{ std::move(trans->m_worldPos) };
        *trans = newTrans;
        trans->m_worldPos = std::move(pos);
      }
    }
    of.AddComponentsToEntity(entity, prefabVar.m_components);
  }

  GE::Debug::ErrorLogger::GetInstance().LogMessage("Entities in scene have been updated with prefab changes");
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

#endif

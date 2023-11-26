#pragma once
#ifndef NO_IMGUI
#include <Singleton/Singleton.h>
#include <unordered_map>
#include <ECS/Entity/Entity.h>
#include <Events/Listener.h>

namespace GE
{
  namespace Prefabs
  {
    class PrefabManager : public Singleton<PrefabManager>, public Events::IEventListener
    {
    public:
      using PrefabVersion = unsigned;
      using EntityPrefabMap = std::unordered_map<ECS::Entity, std::pair<std::string, PrefabVersion>>;


      void AttachPrefab(ECS::Entity entity, std::string prefabName);

      void DetachPrefab(ECS::Entity entity);

      bool DoesEntityHavePrefab(ECS::Entity entity) const;

      /*!*********************************************************************
      \brief
        Gets the prefab an entity was created from. Returns an empty string
        if there isn't one.
      \param entity
        The entity to get the prefab of
      \return
        The prefab an entity was created from and an empty string otherwise
      ************************************************************************/
      EntityPrefabMap::mapped_type const& GetEntityPrefab(ECS::Entity entity) const;

      /*!*********************************************************************
      \brief
        Gets the current version of a prefab
      \param prefab
        The prefab to get the version of
      \return
        The version of the prefab
      ************************************************************************/
      PrefabVersion GetPrefabVersion(std::string const& prefab) const;

      void CreatePrefabFromEntity(ECS::Entity entity, std::string const& name) const;

      void UpdateEntitiesFromPrefab(std::string const& prefab);

      void UpdateAllEntitiesFromPrefab();

      void HandleEvent(Events::Event* event) override;

    private:
      EntityPrefabMap m_entitiesToPrefabs;
      std::unordered_map<std::string, PrefabVersion> m_prefabVersions;
    };
  }
}

#endif

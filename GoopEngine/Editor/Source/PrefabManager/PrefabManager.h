#pragma once
#ifndef NO_IMGUI
#include <Singleton/Singleton.h>
#include <unordered_map>
#include <ECS/Entity/Entity.h>

namespace GE
{
  namespace Prefabs
  {
    class PrefabManager : public Singleton<PrefabManager>
    {
    public:

      void AttachPrefab(ECS::Entity entity, std::string prefabName);

      void DetachPrefab(ECS::Entity entity);

      /*!*********************************************************************
      \brief
        Gets the prefab an entity was created from. Returns an empty string
        if there isn't one.
      \param entity
        The entity to get the prefab of
      \return
        The prefab an entity was created from and an empty string otherwise
      ************************************************************************/
      std::string GetEntityPrefab(ECS::Entity entity) const;

    private:
      using EntityPrefabMap = std::unordered_map<ECS::Entity, std::string>;

      EntityPrefabMap m_entitiesToPrefabs;
    };
  }
}

#endif

/*!*********************************************************************
\file   PrefabManager.h
\author chengen.lau\@digipen.edu
\date   27-November-2023
\brief  
  This file contains the definition of PrefabManager singleton.
  It is responsible for the mapping of entities to the prefabs they
  were created from. This is to allow each instance to be updated by
  any changes made to the prefab itself. The functions below are
  used to facilitate the adding and removing of entities to prefabs,
  as well as the updating of components based on prefabs.

  ** THIS CLASS ONLY RUNS IN THE EDITOR **

  Currently, the PrefabManager will update all instances upon loading
  a scene or saving a prefab (through listening for the event).
  In future, it aims to be optimized through attaching a "version"
  to each prefab so that only outdated entities need to be updated
  with the prefab's components.
  Additionally, an entity's component should also not be updated if
  it was changed externally.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
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
      /*!*********************************************************************
      \brief
        Assigns a prefab to an entity
      \param entity
        The entity to assign the prefab
      \param prefabName
        The name of the prefab
      ************************************************************************/
      void AttachPrefab(ECS::Entity entity, std::string prefabName);

      /*!*********************************************************************
      \brief
        Unsubscribes an entity from prefab updates
      \param entity
        The entity id
      ************************************************************************/
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

      /*!*********************************************************************
      \brief
        This function creates a prefab from an entity's components and saves
        it to the Prefabs directory based on the Assets file path from the
        asset manager.
      \param entity
        The entity to create the prefab from
      \param name
        The name of the new prefab
      ************************************************************************/
      void CreatePrefabFromEntity(ECS::Entity entity, std::string const& name) const;

      /*!*********************************************************************
      \brief
        This function updates all entities associated with the given prefab
      \param prefab
        The prefab to update all entities with
      ************************************************************************/
      void UpdateEntitiesFromPrefab(std::string const& prefab);

      /*!*********************************************************************
      \brief
        This function updates all entities in the map based on their 
        respective prefabs
      ************************************************************************/
      void UpdateAllEntitiesFromPrefab();
      
      /*!*********************************************************************
      \brief
        This function handles the corresponding events the PrefabManager
        subscribed to.

        REMOVE_ENTITY
          - Removes the entity entry from the map if the entity is destroyed

      \param event
        The event to be handled
      ************************************************************************/
      void HandleEvent(Events::Event* event) override;

    private:
      using EntityPrefabMap = std::unordered_map<ECS::Entity, std::string>;

      EntityPrefabMap m_entitiesToPrefabs;
      std::unordered_map<std::string, unsigned> m_prefabVersions;
    };
  }
}

#endif

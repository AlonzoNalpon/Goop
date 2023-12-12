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
  Currently, the PrefabManager attaches a "version" to each prefab so
  that only outdated entities are updated with the prefab's components
  upon loading a scene.

  ** THIS CLASS ONLY RUNS IN THE EDITOR **
  
  Further improvements can be made so that an entity's component 
  should no longer be updated if it was changed externally through
  inspector.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#ifndef IMGUI_DISABLE
#include <Singleton/Singleton.h>
#include <Prefabs/VariantPrefab.h>
#include <unordered_map>
#include <ECS/Entity/Entity.h>
#include <Events/Listener.h>
#include <optional>

namespace GE
{
  namespace Prefabs
  {
    class PrefabManager : public Singleton<PrefabManager>, public Events::IEventListener
    {
    public:
      using PrefabVersion = unsigned;
      using PrefabDataContainer = std::unordered_map<std::string, VariantPrefab>;
      using EntityPrefabMap = std::unordered_map<ECS::Entity, std::pair<std::string, PrefabVersion>>;

      /*!*********************************************************************
    \brief
      Goes through the map and create an entity with the data.
    \param key
      string& (name of the prefab in the m_prefabs map)
    \return
      Entity of the prefab.
    ************************************************************************/
      GE::ECS::Entity SpawnPrefab(const std::string& key);

      /*!*********************************************************************
      \brief
        Gets the file path from the asset manager and loads all prefabs
      ************************************************************************/
      void LoadPrefabsFromFile();

      /*!*********************************************************************
      \brief
        Gets the deserialized data of a prefab in the form of a VariantPrefab
        object. Throws a GE::Debug::Exception if not found.
      \param name
        The name of the prefab
      \return
        The VariantPrefab object
      ************************************************************************/
      VariantPrefab const& GetVariantPrefab(std::string const& name) const;

      /*!*********************************************************************
      \brief
        Reloads a particular prefab given its name
      ************************************************************************/
      void ReloadPrefab(std::string const& name);

      /*!*********************************************************************
      \brief
        Clears the prefab container and loads prefabs from files again
      ************************************************************************/
      void ReloadPrefabs();

      /*!*********************************************************************
      \brief
        Assigns a prefab to an entity
      \param entity
        The entity to assign the prefab
      \param prefab
        std::pair containing the prefab name and version
      ************************************************************************/
      void AttachPrefab(ECS::Entity entity, EntityPrefabMap::mapped_type const& prefab);
      void AttachPrefab(ECS::Entity entity, EntityPrefabMap::mapped_type&& prefab);

      /*!*********************************************************************
      \brief
        Unsubscribes an entity from prefab updates
      \param entity
        The entity id
      ************************************************************************/
      void DetachPrefab(ECS::Entity entity);

      inline void SetPrefabVersion(std::string const& prefab, PrefabVersion version) noexcept { m_prefabVersions[prefab] = version; }

      /*!*********************************************************************
      \brief
        Gets the prefab an entity if it was created from one and std::nullopt
        otherwise
      \param entity
        The entity to get the prefab of
      \return
        std::optional containing the prefab an entity was created from
      ************************************************************************/
      std::optional<EntityPrefabMap::mapped_type> GetEntityPrefab(ECS::Entity entity) const;

      /*!*********************************************************************
      \brief
        Gets the current version of a prefab. If it doesn't exist, an entry
        of 0 will be created and returned.
      \param prefab
        The prefab to get the version of
      \return
        The version of the prefab
      ************************************************************************/
      PrefabVersion GetPrefabVersion(std::string const& prefab);
      
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
      void CreatePrefabFromEntity(ECS::Entity entity, std::string const& name);

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
      EntityPrefabMap m_entitiesToPrefabs;
      std::unordered_map<std::string, PrefabVersion> m_prefabVersions;
      PrefabDataContainer m_prefabs;  // Map of deserialized prefab data in format <name, data>
    };
  }
}

#endif

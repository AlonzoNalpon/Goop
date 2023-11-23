/*!*********************************************************************
\file   ObjectFactory.h
\author loh.j@digipen.edu
\date   28 September 2023
\brief
  Parses in data and stores it in a map to be used in the future.
  
  Object Factory is capable of:
    - SpawnPrefab
        Creates an entity using the prefab data.

    - CreateObject
        Creates an entity with the data from a json file.

    - CloneObject
        Clones an existing entity.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/ComponentTypes.h>
#include <rapidjson/document.h>
#include <ObjectFactory/ObjectStructs.h>
#include <Serialization/Serializer.h>
#include <Component/Components.h>

namespace GE::ObjectFactory
{
  // ObjectFactory singleton
  class ObjectFactory : public Singleton<ObjectFactory>
  {
  public:
    using EntityDataContainer = std::vector<std::pair<ECS::Entity, VariantEntity>>;
    using PrefabDataContainer = std::unordered_map<std::string, VariantPrefab>;

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
      Empties the loaded map of object data.
    ************************************************************************/
    void EmptyMap();
    
    /*!*********************************************************************
    \brief
      Extracts a component from an rttr::variant and adds it to an entity
    ************************************************************************/
    void AddComponentToEntity(ECS::Entity entity, rttr::variant const& compVar) const;

    /*!*********************************************************************
    \brief
      Copies an entity and set the position of it.
    \param
      Entity (Source),
      dVec2&& (Position of the cloned object)
    ************************************************************************/
    void CloneObject(ECS::Entity entity, ECS::Entity parent = ECS::INVALID_ID) const;

    /*!*********************************************************************
    \brief     
      Gets the file path from the asset manager and loads all prefabs
    ************************************************************************/
    void LoadPrefabsFromFile();

    /*!*********************************************************************
    \brief
      Creates the objects from the scene file.
    \param
      std::set<GE::ECS::Entity>& (map of loaded objects to be deleted later)
    ************************************************************************/
    void LoadSceneObjects(std::set<GE::ECS::Entity>& map);

    /*!*********************************************************************
    \brief
      Loads the data from json data into the object map.
    \param
      std::string (filename of the scene.scn file)
    ************************************************************************/
    void LoadSceneJson(std::string const& filename);

    /*!*********************************************************************
    \brief
      Registers components and system for initializing the ECS.
    ************************************************************************/
    void RegisterComponentsAndSystems() const;

    /*!*********************************************************************
    \brief
      Clears the object map.
   ************************************************************************/
    void ClearSceneObjects();

  private:
    /*!*********************************************************************
    \brief
      Clears the prefab container and loads prefabs from files again
    ************************************************************************/
    void ReloadPrefabs();

    /*!*********************************************************************
    \brief
      Loads the data into the class map.
    \param
      String& (filepath of the serialized file)
    ************************************************************************/
    void AddComponentsToEntity(ECS::Entity id, std::vector<rttr::variant> const& components) const;

    /*!*********************************************************************
    \brief
      Clones the component the source entity to the destination entity
    \param
      Entity (Destination),
      Entity (source)
    ************************************************************************/
    void CloneComponents(GE::ECS::Entity destObj, GE::ECS::Entity srcObj) const;

    /*!*********************************************************************
    \brief
      Registers components based on the given signature to a system
      specified by template argument T
    \param sig
      The signature of the components to register
    ************************************************************************/
    template <typename T>
    void RegisterComponentsToSystem(ECS::ComponentSignature sig) const;

    /*!*********************************************************************
    \brief
      This function registers a system to the system given the
      enum. This has to be updated everytime a new system is introduced.
    \param name
      The enum corresponding to the system
    ************************************************************************/
    void RegisterSystemWithEnum(ECS::SYSTEM_TYPES name, ECS::ComponentSignature sig) const;

    /*!*********************************************************************
    \brief
      If the entity has a specific signature, this function will
      set the bit as true
    \param 
      Entity (Entity to be compared to)
      Signature& (SystemSignature object to be set)
      SYSTEM_TYPES (SYSTEM_TYPE)
    ************************************************************************/
    template <typename T, typename Signature>
    void SetBitIfFound(ECS::Entity entity, Signature& sig, ECS::SYSTEM_TYPES type) const
    {
      std::set<ECS::Entity>& entities{ ECS::EntityComponentSystem::GetInstance().GetSystem<T>()->GetEntities() };
      if (entities.find(entity) != entities.end()) { sig[static_cast<unsigned>(type)] = true; }
    }
    
    EntityDataContainer m_deserialized;   // Container of deserialized entity data in format <id, data>
    PrefabDataContainer m_prefabs;        // Map of deserialized prefab data in format <name, data>

  };
  #include "ObjectFactory.tpp"
}

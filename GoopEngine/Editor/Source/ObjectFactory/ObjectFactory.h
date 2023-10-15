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
#include <pch.h>
#include <ECS/ComponentTypes.h>
#include <rapidjson/document.h>
#include <ObjectFactory/ObjectStructs.h>
#include <Serialization/Serialization.h>
#include <Component/Velocity.h>
#include <Component/Transform.h>
#include <Component/Sprite.h>
#include <Component/SpriteAnim.h>
#include <Component/BoxCollider.h>
#include <Component/Tween.h>
#include <Component/Model.h>

namespace GE::ObjectFactory
{
  // ObjectFactory singleton
  class ObjectFactory : public Singleton<ObjectFactory>
  {
  public:
    /*!*********************************************************************
    \brief
      Loads the prefab from the path specified in the config file.
    ************************************************************************/
    void LoadPrefabsFromFile();

    /*!*********************************************************************
    \brief
      Registers components and system for initializing the ECS.
    ************************************************************************/
    void RegisterComponentsAndSystems() const;

    /*!*********************************************************************
    \brief
      Goes through the map and create an entity with the data.
    \param $PARAMS
      string& (name of the prefab in the m_prefabs map)
    \return
      Entity of the prefab.
    ************************************************************************/
    GE::ECS::Entity SpawnPrefab(const std::string& key) const;
    
    /*!*********************************************************************
    \brief
      Creates an entity with the given object.
    \param
      ObjectData (Data to be converted into an entity)
    \return
      Entity (Created entity)
    ************************************************************************/
    GE::ECS::Entity CreateObject(ObjectData data) const;

    /*!*********************************************************************
    \brief
      Copies an entity and set the position of it.
    \param
      Entity (Source),
      dVec2&& (Position of the cloned object)
    ************************************************************************/
    void CloneObject(ECS::Entity obj, const Math::dVec2& newPos);

    /*!*********************************************************************
    \brief
      Loads the data from json data to the entity map.
    \return
      0 if success, 1 if failed
    ************************************************************************/
    int LoadObject() const;

    /*!*********************************************************************
    \brief
      Loads the json file into the map.
    \param
      string& (Path to the json file)
    ************************************************************************/
    void ObjectJsonLoader(const std::string& json_path);

    /*!*********************************************************************
    \brief
      Verify that the object factory deserializes properly
    ************************************************************************/
    static void ObjectFactoryTest();

  private:
    /*!*********************************************************************
    \brief
      Loads the data into the class map.
    \param
      String& (filepath of the serialized file)
    ************************************************************************/
    void DeserializePrefab(const std::string& filepath);

    /*!*********************************************************************
    \brief
      Register the specified object to the specified system.
    \param
      Entity (object to register)
      SystemSignature (System to be registered to)
    ************************************************************************/
    void RegisterObjectToSystems(GE::ECS::Entity object, ECS::SystemSignature signature) const;
    
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
      Gets the system signature of the entity.
    \param 
      Entity (Entity to check)
    \return
      SystemSignature
    ************************************************************************/
    GE::ECS::SystemSignature GetObjectSystemSignature(GE::ECS::Entity obj) const;

    /*!*********************************************************************
    \brief
      Registers components based on the given signature to a system
      specified by template argument T
    \param sig
      The signature of the components to register
    ************************************************************************/
    template <typename T>
    void RegisterComponentsToSystem(ECS::ComponentSignature sig) const
    {
      ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };

      if (IsBitSet(sig, ECS::COMPONENT_TYPES::TRANSFORM))
        ecs.RegisterComponentToSystem<GE::Component::Transform, T>();
      if (IsBitSet(sig, ECS::COMPONENT_TYPES::BOXCOLLIDER))
        ecs.RegisterComponentToSystem<GE::Component::BoxCollider, T>();
      if (IsBitSet(sig, ECS::COMPONENT_TYPES::SPRITEANIM))
        ecs.RegisterComponentToSystem<GE::Component::SpriteAnim, T>();
      if (IsBitSet(sig, ECS::COMPONENT_TYPES::SPRITE))
        ecs.RegisterComponentToSystem<GE::Component::Sprite, T>();
      if (IsBitSet(sig, ECS::COMPONENT_TYPES::MODEL))
        ecs.RegisterComponentToSystem<GE::Component::Model, T>();
      if (IsBitSet(sig, ECS::COMPONENT_TYPES::VELOCITY))
        ecs.RegisterComponentToSystem<GE::Component::Velocity, T>();
      if (IsBitSet(sig, ECS::COMPONENT_TYPES::TWEEN))
        ecs.RegisterComponentToSystem<GE::Component::Tween, T>();
    }

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

    std::map<std::string, ObjectData> m_objects; // Map of objects with pair of name, and ObjectData.
    std::unordered_map<std::string, PrefabData> m_prefabs; // Map of prefabs with pair of name, and PrefabData.
  };
}

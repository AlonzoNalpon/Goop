/*!*********************************************************************
\file       ObjectFactory.h
\author     loh.j\@digipen.edu
\co-author  chengen.lau\@digipen.edu
\date       28 September 2023
\brief
  Contains the definition of the ObjectFactory singleton class, which 
  encapsulates functions and data members to facilitate the creation 
  of objects in the scene. The class serves as a middle-man between
  serialization and the actual entities in the scene. It is responsible 
  for the creation/cloning of entities and adding of components through
  the ECS.

  On load, the ObjectFactory holds the deserialized data for the scene.
  Upon reloading, the objects are loaded from here without the need to
  deserialize again. Only when changing scenes will a full reload be
  required.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <ECS/ComponentTypes.h>
#include <rapidjson/document.h>
#include <ObjectFactory/ObjectStructs.h>
#include <Serialization/Serializer.h>
#include <Component/Components.h>
#include <sstream>

namespace GE::ObjectFactory
{
  // ObjectFactory singleton
  class ObjectFactory : public Singleton<ObjectFactory>
  {
  public:
    using EntityDataContainer = std::vector<std::pair<ECS::Entity, VariantEntity>>;

    /*!*********************************************************************
    \brief
      Empties the loaded map of object data.
    ************************************************************************/
    void EmptyMap();

    /*!*********************************************************************
    \brief
      Creates a copy of an entity along with its components. This also
      duplicates all child entities, which is done through recursive calls
      to this function
    \param entity
      The entity to duplicate
    \param parent
      The parent of the entity. It is set to ECS::INVALID_ID by default,
      indicating no parent.
    \return
    ************************************************************************/
    void CloneObject(ECS::Entity entity, ECS::Entity parent = ECS::INVALID_ID) const;

    /*!*********************************************************************
    \brief
      Creates the objects from the scene file.
    \param
      std::set<GE::ECS::Entity>& (map of loaded objects to be deleted later)
    ************************************************************************/
    void LoadSceneObjects();

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

    /*!*********************************************************************
    \brief
      Extracts a component from an rttr::variant and adds it to an entity
    \param entity
      The entity to add to
    \param compVar
      The component to add
   ************************************************************************/
    void AddComponentToEntity(ECS::Entity entity, rttr::variant const& compVar) const;

    /*!*********************************************************************
    \brief
      Adds all components within a vector of rttr::variants to an entity
    \param id
      Id of the entity
    \param components
      The vector of components
    ************************************************************************/
    void AddComponentsToEntity(ECS::Entity id, std::vector<rttr::variant> const& components) const;

    /*!*********************************************************************
    \brief
      Removes a component from an entity given the rttr::type
    \param compType
      The type of the component
    ************************************************************************/
    void RemoveComponentFromEntity(ECS::Entity entity, rttr::type compType) const;

    /*!*********************************************************************
    \brief
      Returns the component of an entity given the Enumeration type. The
      component is returned in the form of an rttr::variant containing
      a shared_ptr to the object
    \param id
      The id of the entity
    \param type
      The rttr::type of the component
    \return
      An rttr::variant of the component
    ************************************************************************/
    rttr::variant GetEntityComponent(ECS::Entity id, rttr::type const& compType) const;

    /*!*********************************************************************
    \brief
      Returns the components of a given entity in the form of a vector of
      rttr::variants, with each variant holding a component.
    \param id
      The id of the entity
    \return
      An std::vector of components in the form of rttr::variants
    ************************************************************************/
    std::vector<rttr::variant> GetEntityComponents(ECS::Entity id) const;

  private:

    /*!*********************************************************************
    \brief
      Registers components based on the given signature to a system
      specified by template argument T
    \param components
      The vector of components to register to the system
    ************************************************************************/
    template <typename T>
    void RegisterComponentsToSystem(std::vector<rttr::type> const& components) const;

    /*!*********************************************************************
    \brief
      This function registers a system to the system given the type. This 
      has to be updated everytime a new system is introduced.
    \param systemType
      The rttr::type of the system
    \param components
      The vector of components to register to the system
    ************************************************************************/
    void RegisterSystemWithType(rttr::type const& systemType, std::vector<rttr::type> const& components) const;

    EntityDataContainer m_deserialized;   // Container of deserialized entity data in format <id, data>
  };
  #include "ObjectFactory.tpp"
}

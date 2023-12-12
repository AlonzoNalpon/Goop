/*!*********************************************************************
\file   VariantPrefab.h
\author chengen.lau\@digipen.edu
\date   12-December-2023
\brief  
  Contains the definition of the struct encapsulating deserialized
  prefab data. It is used during creation of entities from prefabs and
  when editing prefabs in the prefab editor. The implementation makes
  use of RTTR library to store components as rttr::variant objects.
  Each prefab also allows for multiple layers of components.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <vector>
#include <string>
#include <rttr/type.h>
#include <ECS/Entity/Entity.h>

#define PREFAB_V2

namespace GE::Prefabs
{
  // struct encapsulating deserialized data of a prefab's child components
  // this should only be instantiated for use with a VariantPrefab
  struct PrefabSubData
  {
    PrefabSubData();
    PrefabSubData(std::string name, ECS::Entity id, ECS::Entity parent = BasePrefabId);

    void AddComponent(rttr::variant const& comp) { m_components.emplace_back(comp); }
    void AddComponent(rttr::variant&& comp) { m_components.emplace_back(std::move(comp)); }

    /*!*********************************************************************
    \brief
      Constructs an entity with the data in the current object. The entity
      will be created with its components and is automatically added to
      the ECS.
      This function does not handle the mapping of parent-child relations.
    \return
      The ID of the created entity
    ************************************************************************/
    ECS::Entity Construct() const;

    std::string m_name;
    std::vector<rttr::variant> m_components;
    ECS::Entity m_id, m_parent;

    // id of the first layer of the prefab
    static ECS::Entity const BasePrefabId = 0;
  };
#ifdef PREFAB_V2
  // struct encapsulating deserialized prefab data
  // components are stored in an std::vector of rttr::variants
  struct VariantPrefab
  {
    VariantPrefab() = default;
    VariantPrefab(std::string name, unsigned version = 0);

    inline bool HasChildComponents() const noexcept { return m_objects.empty(); }

    /*!*********************************************************************
    \brief
      Constructs an entity with the data in the current VariantPrefab.
      The entity will be created along with it's relevant hierarchy and
      is automatically added to the ECS.
    \return
      The ID of the created entity
    ************************************************************************/
    ECS::Entity Construct() const;

    void Clear() noexcept;

    std::string m_name;
    std::vector<PrefabSubData> m_objects;
    std::vector<rttr::variant> m_components;
    unsigned m_version;
  };
#else
  // struct encapsulating deserialized prefab data
  // components are stored in an std::vector of rttr::variants
  struct VariantPrefab
  {
    VariantPrefab();
    VariantPrefab(std::string name, unsigned version = 0);

    std::string m_name;
    std::vector<rttr::variant> m_components;
    unsigned m_version;

    void Clear() noexcept;
  };
#endif
}
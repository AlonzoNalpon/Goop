/*!*********************************************************************
\file   Serializer.cpp
\author chengen.lau\@digipen.edu
\date   3-November-2023
\brief  Contians the class encapsulating functions related to
        serialization. Implementation makes use of RTTR reflection
        library. Applies a standard algorithm of recursion
        to work for any type and size for sequential containers. This
        is so that not much code has to be modified when any
        components/classes are changed.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <rapidjson/document.h>
#include <string>
#include <Component/Scripts.h>
#include <Component/Sprite.h>
#include <rttr/type.h>
#include <Prefabs/VariantPrefab.h>

namespace GE
{
  namespace Serialization
  {
    class Serializer
    {
    public:
      // pure static class
      Serializer() = delete;

#ifndef IMGUI_DISABLE
      /*!*********************************************************************
      \brief  
        Serializes a VariantPrefab object into a json file. The VariantPrefab
        encapsulates a proxy entity by storing components in a vector each
        with the form of an rttr::variant.
      \param prefab
        The VariantPrefab object
      \param filename
        The name of the output file
      ************************************************************************/
      static void SerializeVariantToPrefab(Prefabs::VariantPrefab const& prefab, std::string const& filename);
#endif

      /*!*********************************************************************
      \brief
        Serializes an rttr::variant object of any type into a json file.
        Current implementation can support almost any form of sequential
        containers such as std::vector.
      \param filename
        The name of the ouput file
      \param object
        The rttr::variant object containing the object to serialize
      ************************************************************************/
      static void SerializeAny(std::string const& filename, rttr::variant object);

      /*!*********************************************************************
      \brief
        Serializes the current system order and their components into a json
        file
      \param json
        The name of the output file
      ************************************************************************/
      static void SerializeSystems(std::string const& json);

      /*!*********************************************************************
      \brief
        Serializes a scene into a json file. Calls other private function
        to serialize each entity, their components and their members.
      \param filename
        The name of the output file
      \return
      ************************************************************************/
      static void SerializeScene(std::string const& filename);

      /*!*********************************************************************
      \brief
        Helper function to serialize a component into a rapidjson::Value.
        Called by SerializeScene
      \param var
        The rttr::variant of the object
      \param allocator
        The document's allocator
      \return
        The resulting rapidjson::Value object
      ************************************************************************/
      static rapidjson::Value SerializeComponent(rttr::variant const& var, rapidjson::Document::AllocatorType& allocator);
    private:
      // helper functions
      /*!*********************************************************************
      \brief
        Serializes an rttr::variant into a rapidjson::Value object based
        on its type. (Whether its a C basic type, Enum or class type)
      \param object
        The object to serialize
      \param allocator
        The document's allocator
      \return
        The resulting rapidjson::Value object
      ************************************************************************/
      static rapidjson::Value SerializeBasedOnType(rttr::variant const& object, rapidjson::Document::AllocatorType& allocator);

      /*!*********************************************************************
      \brief
        Serializes an rttr::variant containing a basic C type
      \param valueType
        The rttr::type of the object
      \param value
        The rttr::variant of the object
      \param allocator
        The document's allocator
      \return
        The resulting rapidjson::Value object        
      ************************************************************************/
      static rapidjson::Value SerializeBasicTypes(rttr::type const& valueType,
        rttr::variant const& value, rapidjson::Document::AllocatorType& allocator);

      /*!*********************************************************************
      \brief
        Serializes an rttr::variant containing an enumeration type
      \param valueType
        The rttr::type of the object
      \param value
        The rttr::variant of the object
      \param allocator
        The document's allocator
      \return
        The resulting rapidjson::Value object
      ************************************************************************/
      static rapidjson::Value SerializeEnumType(rttr::type const& valueType, rttr::variant const& object, rapidjson::Document::AllocatorType& allocator);

      /*!*********************************************************************
      \brief
        Serializes an rttr::variant containing a user-defined or class type
      \param valueType
        The rttr::type of the object
      \param value
        The rttr::variant of the object
      \param allocator
        The document's allocator
      \return
        The resulting rapidjson::Value object
      ************************************************************************/
      static rapidjson::Value SerializeClassTypes(rttr::type const& valueType,
        rttr::variant const& value, rapidjson::Document::AllocatorType& allocator);

      /*!*********************************************************************
      \brief
        Serializes an rttr::variant containing a sequential container type
        (such as std::vector). Makes use of recursion to serialize a
        container till it reaches the base element
      \param object
        The rttr::variant of the object
      \param jsonArray
        The rapidjson::Value object to serialize into
      \param allocator
        The document's allocator
      ************************************************************************/
      static void SerializeSequentialContainer(rttr::variant const& object, rapidjson::Value& jsonArray, rapidjson::Document::AllocatorType& allocator);
      
      /*!*********************************************************************
      \brief
        Serializes an rttr::variant containing an associative container type
        (such as std::map)
      \param object
        The rttr::variant of the object
      \param jsonArray
        The rapidjson::Value object to serialize into
      \param allocator
        The document's allocator
      ************************************************************************/
      static void SerializeAssociativeContainer(rttr::variant const& object, rapidjson::Value& jsonArray, rapidjson::Document::AllocatorType& allocator);

      /*!*********************************************************************
      \brief
        Serializes an entity into a rapidjson::Value object containing its
        components and data members.
      \param id
        The id of the entity
      \param childIDs
        The vector of child ids for the entity
      \param allocator
        The allocator of the rapidjson::Document object
      \return
        The resulting radpidjson::Value object containing the entity data
      ************************************************************************/
      static rapidjson::Value SerializeEntity(ECS::Entity id, rapidjson::Document::AllocatorType& allocator);
      
      /*!*********************************************************************
      \brief
        
      \param value

      \param components

      \param allocator
       
      ************************************************************************/
      static void SerializeVariantComponents(rapidjson::Value& value, std::vector<rttr::variant> const& components, rapidjson::Document::AllocatorType& allocator);

      /*!*********************************************************************
      \brief
        The conversion function for Component::Scripts's m_scriptMap
        to rapidjson::Value for serialization
      \param rhs
        The data member of the component to serialize
      \param allocator
        The rapidjson::Document's allocator
      \return
        The serialized json value object of the script names in a rapidjson
        array
      ************************************************************************/
      static rapidjson::Value SerializeScriptMap(std::vector<std::pair<std::string, GE::MONO::ScriptInstance>> const& rhs,
        rapidjson::Document::AllocatorType& allocator);

      /*!*********************************************************************
      \brief
        The conversion function for Component::Tween's m_tweens
        to rapidjson::Value for serialization
      \param tweens
        The std::deque of Math::dVec3s
      \param allocator
        The document's allocator
      \return
        The resulting rapidjson::Value object
      ************************************************************************/
      static rapidjson::Value SerializeTweenQueue(std::deque<Math::dVec3> tweens, rapidjson::Document::AllocatorType& allocator);

      /*!*********************************************************************
      \brief
        The conversion functoin for Component::Sprite to rapidjson::Value
      \param data
        The Component::Sprite's data
      \param allocator
        The document's allocator
      \return
        The resulting rapidjson::Value object
      ************************************************************************/
      static rapidjson::Value SerializeSpriteComponent(Component::Sprite const& data, rapidjson::Document::AllocatorType& allocator);
    };

  } // namespace Serialization
} // namespace GE

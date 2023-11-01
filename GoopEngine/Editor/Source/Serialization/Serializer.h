/*!*********************************************************************
\file   Serialization.h
\author chengen.lau\@digipen.edu
\date   15-October-2023
\brief  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Singleton/Singleton.h>
#include <rapidjson/document.h>
#include <string>
#include <Component/ScriptHandler.h>
#include <Component/Sprite.h>
#include <rttr/type.h>

namespace GE
{
  namespace Serialization
  {
    class Serializer : public Singleton<Serializer>
    {
    public:
      static const char JsonNameKey[], JsonParentKey[], JsonChildEntitiesKey[], JsonComponentsKey[];

      void SerializeAny(std::string const& filename, rttr::variant object);

      rttr::instance GetEntityComponent(ECS::Entity id, ECS::COMPONENT_TYPES type);

      void SerializeSystems(std::string const& json);

      void SerializeScene(std::string const& filename);

      rapidjson::Value SerializeEntity(ECS::Entity id, std::vector<ECS::Entity> const& childIDs, rapidjson::Document::AllocatorType& allocator);

      void Test();

    private:
      std::unordered_map<ECS::Entity, ECS::Entity> m_oldToNewIDs;

      // helper functions
      rapidjson::Value SerializeBasedOnType(rttr::variant const& object, rapidjson::Document::AllocatorType& allocator);

      rapidjson::Value SerializeBasicTypes(rttr::type const& valueType,
        rttr::variant const& value, rapidjson::Document::AllocatorType& allocator);

      rapidjson::Value SerializeEnumType(rttr::type const& valueType, rttr::variant const& object, rapidjson::Document::AllocatorType& allocator);

      rapidjson::Value SerializeClassTypes(rttr::type const& valueType,
        rttr::variant const& value, rapidjson::Document::AllocatorType& allocator);

      void SerializeSequentialContainer(rttr::variant const& object, rapidjson::Value& jsonArray, rapidjson::Document::AllocatorType& allocator);

      rapidjson::Value SerializeComponent(rttr::instance instance, rapidjson::Document::AllocatorType& allocator);
      
      /*!*********************************************************************
      \brief
        The conversion function for Component::ScriptHandler's m_scriptMap
        to rapidjson::Value for serialization
      \param rhs
        The data member of the component to serialize
      \param allocator
        The rapidjson::Document's allocator
      \return
        The serialized json value object of the script names in a rapidjson
        array
    ************************************************************************/
      rapidjson::Value SerializeScriptMap(std::map<std::string, GE::MONO::Script> const& rhs,
        rapidjson::Document::AllocatorType& allocator);

      rapidjson::Value SerializeTweenQueue(std::deque<Math::dVec3> tweens, rapidjson::Document::AllocatorType& allocator);

      rapidjson::Value SerializeSpriteComponent(Component::Sprite const& data, rapidjson::Document::AllocatorType& allocator);
    };

  } // namespace Serialization
} // namespace GE

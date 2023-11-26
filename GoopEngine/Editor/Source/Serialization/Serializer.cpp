/*!*********************************************************************
\file   Serializer.cpp
\author chengen.lau\@digipen.edu
\date   3-November-2023
\brief  Contians the class encapsulating functions related to 
        serialization Implementation makes use of RTTR reflection
        library. Applies a standard algorithm of recursion 
        to work for any type and size for sequential containers. This
        is so that not much code has to be modified when any 
        components/classes are changed.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "Serializer.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <Component/Components.h>
#include <ObjectFactory/ObjectFactory.h>
#include <rttr/enumeration.h>
#ifndef IMGUI_DISABLE
#include <PrefabManager/PrefabManager.h>
#endif

namespace GE
{
  namespace Serialization
  {
    // Names of Keys used when serializing to file
    // Both Serializer and Deserializer uses these to determine the name of the keys
    const char Serializer::JsonNameKey[]          = "Name";
    const char Serializer::JsonIdKey[]            = "ID";
    const char Serializer::JsonEntityStateKey[]   = "isActive";
    const char Serializer::JsonParentKey[]        = "Parent";
    const char Serializer::JsonChildEntitiesKey[] = "Child Entities";
    const char Serializer::JsonComponentsKey[]    = "Components";
    const char Serializer::JsonPrefabKey[]        = "Prefab";
    const char Serializer::JsonAssociativeKey[] = "key", Serializer::JsonAssociativeValue[] = "value";
    const char Serializer::ScriptFieldInstListTypeKey[] = "type";

#ifndef IMGUI_DISABLE
    void Serializer::SerializeVariantToPrefab(ObjectFactory::VariantPrefab const& prefab, std::string const& filename)
    {
      std::ofstream ofs{ filename };
      if (!ofs)
      {
        GE::Debug::ErrorLogger::GetInstance().LogError("Unable to serialize scene into " + filename);
        return;
      }

      rapidjson::Document document{ rapidjson::kObjectType };
      rapidjson::Value name;
      name.SetString(prefab.m_name.c_str(), document.GetAllocator());

      rapidjson::Value compArray{ rapidjson::kArrayType };
      for (rttr::variant const& comp : prefab.m_components) 
      {
        rapidjson::Value compName;
        compName.SetString(comp.get_type().get_wrapped_type().get_raw_type().get_name().to_string().c_str(), document.GetAllocator());
        rapidjson::Value compJson{ rapidjson::kObjectType };
        if (comp.get_type().get_wrapped_type() == rttr::type::get<Component::SpriteAnim*>())
        {
          Component::SpriteAnim const& sprAnim{ *comp.get_value<Component::SpriteAnim*>() };
          rapidjson::Value key, val;
          rapidjson::Value obj{ rapidjson::kObjectType };
          key.SetString("name", document.GetAllocator());
          val.SetString(Graphics::GraphicsEngine::GetInstance().animManager.GetAnimName(sprAnim.animID).c_str(), document.GetAllocator());
          obj.AddMember(key, val, document.GetAllocator());
          compJson.AddMember(compName.Move(), obj, document.GetAllocator());
        }
        else
        {
          compJson.AddMember(compName.Move(), SerializeBasedOnType(comp, document.GetAllocator()), document.GetAllocator());
        }
        if (!compJson.ObjectEmpty())
          compArray.PushBack(compJson, document.GetAllocator());
      }
      document.AddMember(JsonNameKey, name.Move(), document.GetAllocator());
      document.AddMember(JsonComponentsKey, compArray.Move(), document.GetAllocator());

      rapidjson::OStreamWrapper osw{ ofs };
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      document.Accept(writer);

      ofs.close();
    }
#endif

    void Serializer::SerializeAny(std::string const& filename, rttr::variant object)
    {
      std::ofstream ofs{ filename };
      if (!ofs)
      {
        GE::Debug::ErrorLogger::GetInstance().LogError("Unable to serialize scene into " + filename);
        return;
      }

      rapidjson::Document document{ rapidjson::kArrayType };
      rapidjson::Value data{ SerializeBasedOnType(object, document.GetAllocator()) };
      document.CopyFrom(data.Move(), document.GetAllocator());

      rapidjson::OStreamWrapper osw{ ofs };
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      document.Accept(writer);

      // clean up
      ofs.close();
    }

    rapidjson::Value Serializer::SerializeEntity(ECS::Entity id, rapidjson::Document::AllocatorType & allocator)
    {
      ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
      rapidjson::Value entity{ rapidjson::kObjectType };
      // serialize name
      rapidjson::Value entityName{};
      entityName.SetString(ecs.GetEntityName(id).c_str(), allocator);
      entity.AddMember(JsonNameKey, entityName.Move(), allocator);

      // serialize entity id
      rapidjson::Value jsonId{};
      jsonId.SetUint(id);
      entity.AddMember(JsonIdKey, jsonId.Move(), allocator);

#ifndef IMGUI_DISABLE
      // serialize prefab created from
      rapidjson::Value prefabJson{ rapidjson::kNullType };
      std::string const prefabName{ Prefabs::PrefabManager::GetInstance().GetEntityPrefab(id) };
      if (!prefabName.empty()) { prefabJson.SetString(prefabName.c_str(), allocator); }
      entity.AddMember(JsonPrefabKey, prefabJson.Move(), allocator);
#endif

      // serialize state
      rapidjson::Value jsonState{};
      jsonState.SetBool(ecs.GetIsActiveEntity(id));
      entity.AddMember(JsonEntityStateKey, jsonState.Move(), allocator);

      // serialize parent id
      rapidjson::Value jsonParent{ rapidjson::kNullType };
      ECS::Entity const parentID{ ecs.GetParentEntity(id) };
      if (parentID != ECS::INVALID_ID)
      {
        jsonParent.SetUint(parentID);
      }
      entity.AddMember(JsonParentKey, jsonParent.Move(), allocator);

      // serialize child entities
      rapidjson::Value childrenArr{ rapidjson::kArrayType };
      for (ECS::Entity const& child : ecs.GetChildEntities(id))
      {
        rapidjson::Value childJson{};
        childJson.SetUint(child);
        childrenArr.PushBack(childJson.Move(), allocator);
      }
      entity.AddMember(JsonChildEntitiesKey, childrenArr.Move(), allocator);

      rapidjson::Value compArr{ rapidjson::kArrayType };
      for (ECS::COMPONENT_TYPES i{ static_cast<ECS::COMPONENT_TYPES>(0) }; i < ECS::COMPONENT_TYPES::COMPONENTS_TOTAL; ++i)
      {
        rttr::variant compVar{ GetEntityComponent(id, i) };
        
        // skip if component wasn't found
        if (!compVar.is_valid()) { continue; }

        rapidjson::Value comp{ SerializeComponent(compVar, allocator) };
        compArr.PushBack(comp, allocator);
      }
      entity.AddMember(JsonComponentsKey, compArr, allocator);
      return entity;
    }

    void Serializer::SerializeScene(std::string const& filename)
    {
      std::ofstream ofs{ filename };
      if (!ofs)
      {
        GE::Debug::ErrorLogger::GetInstance().LogError("Unable to serialize scene into " + filename);
      }
      ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
      
      rapidjson::Document document{ rapidjson::kArrayType };
      for (ECS::Entity entity : ecs.GetEntities())
      {
        rapidjson::Value entityJson{ SerializeEntity(entity, document.GetAllocator())};
        document.PushBack(entityJson, document.GetAllocator());
      }

      rapidjson::OStreamWrapper osw{ ofs };
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      document.Accept(writer);

      // clean up
      ofs.close();
    }

    void Serializer::SerializeSystems(std::string const& json)
    {
      rapidjson::Document document{};
      std::ofstream ofs{ json };
      if (!ofs)
      {
        throw Debug::Exception<std::ifstream>(Debug::LEVEL_ERROR, ErrMsg("Unable to create output file " + json));
      }
      
      document.SetObject();
      std::map<int, const char*> const& systems{ ECS::EntityComponentSystem::GetInstance().GetSystemIndexes() };
      std::unordered_map<const char*, ECS::ComponentSignature> const& signatures{ ECS::EntityComponentSystem::GetInstance().GetSystemSignatures() };
      // loop through systems
      for (auto const& elem : systems)
      {
        rapidjson::Value compArr{ rapidjson::kArrayType };

        // get component names and append to rapidjson array
        auto const iter{ signatures.find(elem.second) };
        if (iter != signatures.cend())
        {
          std::vector<std::string> components{ ECS::ComponentSignatureToString(iter->second) };
          for (std::string const& str : components)
          {
            rapidjson::Value compName{ str.c_str(), document.GetAllocator() };
            compArr.PushBack(compName, document.GetAllocator());
          }
        }
        else
        {
          std::ostringstream oss{};
          oss << "Unable to find " << elem.second << " in system signatures";
          Debug::ErrorLogger::GetInstance().LogError(oss.str());
        }
        // truncate front part of system class type name to extract the system name 
        // (extract "PlayerControllerSystem" from "class GE::Systems::PlayerControllerSystem")
        std::string const sysName{ elem.second };
        rapidjson::Value key{ sysName.substr(sysName.find_last_of(':') + 1, std::string::npos).c_str(), document.GetAllocator()};
        document.AddMember(key, compArr, document.GetAllocator());
      }

      rapidjson::OStreamWrapper osw{ ofs };
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      document.Accept(writer);
      ofs.close();
    }

    rttr::variant Serializer::GetEntityComponent(ECS::Entity id, ECS::COMPONENT_TYPES type)
    {
      ECS::EntityComponentSystem& ecs{ ECS::EntityComponentSystem::GetInstance() };
      switch (type)
      {
      case ECS::COMPONENT_TYPES::TRANSFORM:
      {
        return ecs.HasComponent<Component::Transform>(id) ? std::make_shared<Component::Transform>(*ecs.GetComponent<Component::Transform>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::BOX_COLLIDER:
      {
        return ecs.HasComponent<Component::BoxCollider>(id) ? std::make_shared<Component::BoxCollider>(*ecs.GetComponent<Component::BoxCollider>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::VELOCITY:
      {
        return ecs.HasComponent<Component::Velocity>(id) ? std::make_shared<Component::Velocity>(*ecs.GetComponent<Component::Velocity>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::SCRIPTS:
      {
        return ecs.HasComponent<Component::Scripts>(id) ? std::make_shared<Component::Scripts>(*ecs.GetComponent<Component::Scripts>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::SPRITE:
      {
        return ecs.HasComponent<Component::Sprite>(id) ? std::make_shared<Component::Sprite>(*ecs.GetComponent<Component::Sprite>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::SPRITE_ANIM:
      {
        return ecs.HasComponent<Component::SpriteAnim>(id) ? std::make_shared<Component::SpriteAnim>(*ecs.GetComponent<Component::SpriteAnim>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::TWEEN:
      {
        return ecs.HasComponent<Component::Tween>(id) ? std::make_shared<Component::Tween>(*ecs.GetComponent<Component::Tween>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::ENEMY_AI:
      {
        return ecs.HasComponent<Component::EnemyAI>(id) ? std::make_shared<Component::EnemyAI>(*ecs.GetComponent<Component::EnemyAI>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::DRAGGABLE:
      {
        return ecs.HasComponent<Component::Draggable>(id) ? std::make_shared<Component::Draggable>(*ecs.GetComponent<Component::Draggable>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::TEXT:
      {
        return ecs.HasComponent<Component::Text>(id) ? std::make_shared<Component::Text>(*ecs.GetComponent<Component::Text>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::AUDIO:
      {
        return ecs.HasComponent<Component::Audio>(id) ? std::make_shared<Component::Audio>(*ecs.GetComponent<Component::Audio>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::GE_BUTTON:
      {
        return ecs.HasComponent<Component::GE_Button>(id) ? std::make_shared<Component::GE_Button>(*ecs.GetComponent<Component::GE_Button>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::ANCHOR:
      {
        return ecs.HasComponent<Component::Anchor>(id) ? std::make_shared<Component::Anchor>(*ecs.GetComponent<Component::Anchor>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::CARD:
      {
        return ecs.HasComponent<Component::Card>(id) ? std::make_shared<Component::Card>(*ecs.GetComponent<Component::Card>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::CARD_HOLDER:
      {
        return ecs.HasComponent<Component::CardHolder>(id) ? std::make_shared<Component::CardHolder>(*ecs.GetComponent<Component::CardHolder>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::CARD_HOLDER_ELEM:
      {
        return ecs.HasComponent<Component::CardHolderElem>(id) ? std::make_shared<Component::CardHolderElem>(*ecs.GetComponent<Component::CardHolderElem>(id)) : rttr::variant();
      }
      case ECS::COMPONENT_TYPES::GAME:
      {
        return ecs.HasComponent<Component::Game>(id) ? std::make_shared<Component::Game>(*ecs.GetComponent<Component::Game>(id)) : rttr::variant();
      }
      default:
      {
        std::ostringstream oss{};
        std::string const enumString = rttr::type::get<ECS::COMPONENT_TYPES>().get_enumeration().value_to_name(type).to_string();
        oss << "Trying to get unsupported component type (" << enumString << ") from Entity " << id;
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        return rttr::variant();
      }
      }
    }

    rapidjson::Value Serializer::SerializeScriptMap(std::vector<std::pair<std::string, MONO::ScriptInstance>> const& scripts, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value ret{ rapidjson::kArrayType };
      for (auto const& [s1, s2] : scripts)
      {
        rapidjson::Value jsonVal{};
        jsonVal.SetString(s1.c_str(), allocator);
        ret.PushBack(jsonVal, allocator);
      }
      return ret;
    }

    rapidjson::Value Serializer::SerializeTweenQueue(std::deque<Math::dVec3> tweens, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value ret{ rapidjson::kArrayType };
      while (!tweens.empty())
      {
        rapidjson::Value jsonVal{};
        jsonVal.SetString(tweens.front().ToString().c_str(), allocator);
        ret.PushBack(jsonVal, allocator);
        tweens.pop_front();
      }
      return ret;
    }

    rapidjson::Value Serializer::SerializeSpriteComponent(Component::Sprite const& data, rapidjson::Document::AllocatorType& allocator)
    {
      Graphics::GraphicsEngine const& gE{ Graphics::GraphicsEngine::GetInstance() };
      GLuint const texHandle{ gE.textureManager.GetTexture(data.m_spriteData.texture).textureHandle };
      rapidjson::Value jsonVal{};
      jsonVal.SetString(gE.textureManager.GetTextureName(texHandle).c_str(), allocator);

      return jsonVal;
    }

    rapidjson::Value Serializer::SerializeBasicTypes(rttr::type const& valueType, rttr::variant const& value, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value jsonVal{ rapidjson::kNullType };
      if (valueType == rttr::type::get<int>())
        jsonVal.SetInt(value.to_int());
      else if (valueType == rttr::type::get<double>())
        jsonVal.SetDouble(value.to_double());
      else if (valueType == rttr::type::get<const char*>())
        jsonVal.SetString(value.to_string().c_str(), allocator);
      else if (valueType == rttr::type::get<bool>())
        jsonVal.SetBool(value.to_bool());
      else if (valueType == rttr::type::get<unsigned>())
        jsonVal.SetUint(value.to_uint32());
      else if (valueType == rttr::type::get<char>())
        jsonVal.SetBool(value.to_bool());
      else if (valueType == rttr::type::get<float>())
        jsonVal.SetFloat(value.to_float());
      else if (valueType == rttr::type::get<uint64_t>())
        jsonVal.SetUint64(value.to_uint64());
      else if (valueType == rttr::type::get<uint16_t>())
        jsonVal.SetInt(value.to_uint16());
      else if (valueType == rttr::type::get<uint8_t>())
        jsonVal.SetInt(value.to_uint8());
      else
      {
        std::ostringstream oss{};
        oss << "Trying to deserialize unsupported basic type: " << valueType.get_name() << " with value " << value.to_string();
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        jsonVal.SetNull();
      }

      return jsonVal;
    }

    rapidjson::Value Serializer::SerializeClassTypes(rttr::type const& valueType, rttr::variant const& value, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value jsonVal{ rapidjson::kObjectType };
      if (valueType == rttr::type::get<GE::Math::dVec3>())
      {
        jsonVal.SetString(valueType.get_method("ToString").invoke(value).to_string().c_str(),
          allocator);
      }
      else if (valueType == rttr::type::get<GE::Math::dVec2>())
      {
        jsonVal.SetString(valueType.get_method("ToString").invoke(value).to_string().c_str(),
          allocator);
      }
      else if (valueType == rttr::type::get<std::string>())
      {
        jsonVal.SetString(value.to_string().c_str(), allocator);
      }
      else if (valueType == rttr::type::get<GE::Math::Vec2>())
      {
        jsonVal.SetString(valueType.get_method("ToString").invoke(value).to_string().c_str(),
          allocator);
      }
      else if (valueType == rttr::type::get<std::deque<Math::dVec3>>())
      {
        jsonVal = SerializeTweenQueue(value.get_value<std::deque<Math::dVec3>>(), allocator);
      }
      else if (valueType == rttr::type::get<Component::DragForce>())
      {
        for (auto const& prop : value.get_type().get_properties())
        {
          rapidjson::Value innerProp{ SerializeBasedOnType(prop.get_value(value), allocator) };
          rapidjson::Value innerKey{ prop.get_name().to_string().c_str(), allocator };
          jsonVal.AddMember(innerKey, innerProp, allocator);
        }

        return jsonVal;
      }
      else if (valueType == rttr::type::get<std::vector<Component::LinearForce>>())
      {
        jsonVal.SetArray();
        for (rttr::variant const& force : value.get_value<std::vector<Component::LinearForce>>())
        {
          //rttr::instance forceInstance{ force };
          rapidjson::Value forceJson{ rapidjson::kObjectType };
          for (auto const& prop : force.get_type().get_properties())
          {
            rapidjson::Value innerVal{ SerializeBasedOnType(prop.get_value(value), allocator) };
            rapidjson::Value jsonKey{ prop.get_name().to_string().c_str(), allocator };
            forceJson.AddMember(jsonKey, innerVal, allocator);
          }
          jsonVal.PushBack(forceJson, allocator);
        }

        return jsonVal;
      }
      else if (valueType == rttr::type::get<Component::SpriteAnim>())
      {
       /*for (auto const& prop : valueType.get_properties())
        {
          rapidjson::Value propVal{ rapidjson::kNullType };
          rapidjson::Value propKey{ prop.get_name().to_string().c_str(), allocator };

          propVal = SerializeBasedOnType(prop.get_value(value), allocator);
          jsonVal.AddMember(propKey, propVal, allocator);
        }*/
        rapidjson::Value key, val;
        key.SetString("name", allocator);
        val.SetString(Graphics::GraphicsEngine::GetInstance().animManager.GetAnimName(valueType.get_property("name").get_value(value).to_uint32()).c_str(), allocator);
        jsonVal.AddMember(key, val, allocator);
      }
      else
      {
        jsonVal.SetObject();
        for (auto const& prop : valueType.get_properties())
        {
          rapidjson::Value propVal{ rapidjson::kNullType };
          rapidjson::Value propKey{ prop.get_name().to_string().c_str(), allocator };

          propVal = SerializeBasedOnType(prop.get_value(value), allocator);
          jsonVal.AddMember(propKey, propVal, allocator);
        }
      }

      return jsonVal;
    }

    rapidjson::Value Serializer::SerializeComponent(rttr::variant const& var, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value comp{ rapidjson::kObjectType };
      rapidjson::Value compInner{ rapidjson::kObjectType };
      rapidjson::Value compName{};
      // extract the raw type of the object
      rttr::type const varType{ var.get_type().is_wrapper() ? var.get_type().get_wrapped_type().get_raw_type() :
        var.get_type().is_pointer() ? var.get_type().get_raw_type() : var.get_type() };
      compName.SetString(varType.get_name().to_string().c_str(), allocator);

      if (varType == rttr::type::get<Component::SpriteAnim>())
      {
        Component::SpriteAnim const& sprAnim{ *var.get_value<Component::SpriteAnim*>() };
        rapidjson::Value jsonAnimVal{};
        jsonAnimVal.SetString(Graphics::GraphicsEngine::GetInstance().animManager.GetAnimName(sprAnim.animID).c_str(), allocator);
        compInner.AddMember("name", jsonAnimVal, allocator);
      }

      for (auto const& prop : varType.get_properties())
      {
        rapidjson::Value jsonVal{ rapidjson::kNullType };
        rapidjson::Value jsonKey{ prop.get_name().to_string().c_str(), allocator };

        rttr::variant value{ prop.get_value(var) };

        if (prop.get_type().is_class())  // else if custom types
        {
          // Handling special cases here (e.g. Scripts's script map)
          if (value.get_type() == rttr::type::get<std::map<std::string, GE::MONO::ScriptInstance>>())
          {
            jsonVal = SerializeScriptMap(value.get_value<std::vector<std::pair<std::string, GE::MONO::ScriptInstance>> const&>(), allocator);
          }
          else
          {
            jsonVal = SerializeBasedOnType(value, allocator).Move();
          }
        }
        else
        {
          jsonVal = SerializeBasedOnType(value, allocator).Move();
        }

        compInner.AddMember(jsonKey, jsonVal, allocator);
      }
      comp.AddMember(compName, compInner, allocator);

      return comp;
    }

    rapidjson::Value Serializer::SerializeEnumType(rttr::type const& valueType, rttr::variant const& object, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value strJson{ rapidjson::kNullType };
      strJson.SetString(valueType.get_enumeration().value_to_name(object).to_string().c_str(), allocator);

      return strJson;
    }

    rapidjson::Value Serializer::SerializeBasedOnType(rttr::variant const& object, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value value{ rapidjson::kNullType };
      // if object is of basic type
      rttr::type valueType{ object.get_type() };
      rttr::type wrappedType{ valueType.is_wrapper() ? valueType.get_wrapped_type() : valueType };

      if (wrappedType.is_arithmetic())
      {
        value.SetObject();
        value = SerializeBasicTypes(wrappedType, object, allocator).Move();
      }
      else if (wrappedType.is_enumeration()) // if enum
      {
        value.SetObject();
        value = SerializeEnumType(wrappedType, object, allocator).Move();
      }
      else if (wrappedType.is_sequential_container())
      {
        value.SetArray();
        SerializeSequentialContainer(object, value, allocator);
      }
      else if (wrappedType.is_associative_container())
      {
        value.SetArray();
        SerializeAssociativeContainer(object, value, allocator);
      }
      else  // if class type
      {
        value.SetObject();
        value = SerializeClassTypes(wrappedType, object, allocator).Move();
      }

      return value;
    }

    void Serializer::SerializeSequentialContainer(rttr::variant const& object,
      rapidjson::Value& jsonArray, rapidjson::Document::AllocatorType& allocator)
    {
      auto containerIter = object.create_sequential_view();
      for (const auto& elem : containerIter)
      {
        // if underlying element is still a container, recurse
        if (elem.is_sequential_container())
        {
          SerializeSequentialContainer(elem, jsonArray, allocator);
        }
        else
        {
          rttr::variant wrappedVal = elem.extract_wrapped_value();
          // else serialize element normally
          jsonArray.PushBack(SerializeBasedOnType(wrappedVal, allocator).Move(), allocator);
        }
      }
    }

    void Serializer::SerializeAssociativeContainer(rttr::variant const& object,
      rapidjson::Value& jsonArray, rapidjson::Document::AllocatorType& allocator)
    {
      auto containerIter = object.create_associative_view();
      rttr::string_view const key_name{ JsonAssociativeKey }, value_name{ JsonAssociativeValue };

      if (containerIter.is_key_only_type())
      {
        for (auto const& elem : containerIter)
        {
          jsonArray.PushBack(SerializeBasedOnType(elem.first, allocator).Move(), allocator);
        }
      }
      else
      {
        for (auto const& elem : containerIter)
        {
          rapidjson::Value jsonEntry{ rapidjson::kObjectType };
          rapidjson::Value jsonKey{ SerializeBasedOnType(elem.first, allocator) },
            jsonValue{ SerializeBasedOnType(elem.second, allocator) };

          jsonEntry.AddMember(JsonAssociativeKey, jsonKey, allocator);
          jsonEntry.AddMember(JsonAssociativeKey, jsonValue, allocator);
          // else serialize element normally
          jsonArray.PushBack(jsonEntry.Move(), allocator);
        }
      }
    }
  } // namespace Serialization
} // namespace GE
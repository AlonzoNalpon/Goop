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
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "Serializer.h"
#include <Serialization/JsonKeys.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <Component/Components.h>
#include <ObjectFactory/ObjectFactory.h>
#include <rttr/enumeration.h>
#ifndef IMGUI_DISABLE
#include <Prefabs/PrefabManager.h>
#endif

namespace GE
{
  namespace Serialization
  {
#ifndef IMGUI_DISABLE
    void Serializer::SerializeVariantToPrefab(Prefabs::VariantPrefab const& prefab, std::string const& filename)
    {
      std::ofstream ofs{ filename };
      if (!ofs)
      {
        GE::Debug::ErrorLogger::GetInstance().LogError("Unable to serialize prefab into " + filename);
        return;
      }

      rapidjson::Document document{ rapidjson::kObjectType };
      auto& allocator{ document.GetAllocator() };

      {
        // serialize the base layer of the prefab
        rapidjson::Value nameJson{ prefab.m_name.c_str(), allocator };
        rapidjson::Value verJson{ prefab.m_version }, activeJson{ prefab.m_isActive };

        document.AddMember(JsonNameKey, nameJson.Move(), allocator);
        document.AddMember(JsonPfbActiveKey, activeJson.Move(), allocator);
        document.AddMember(JsonPfbVerKey, verJson.Move(), allocator);
      }

      SerializeVariantComponents(document,prefab.m_components, allocator);

      // serialize nested components if prefab has multiple layers
      rapidjson::Value subDataJson{ rapidjson::kArrayType };
      for (Prefabs::PrefabSubData const& obj : prefab.m_objects)
      {
        rapidjson::Value objJson{ rapidjson::kObjectType };
        {
          rapidjson::Value idJson{ obj.m_id }, activeJson{ obj.m_isActive };
          rapidjson::Value subNameJson{ obj.m_name.c_str(), allocator };
          rapidjson::Value parentJson{ obj.m_parent };

          objJson.AddMember(JsonIdKey, idJson.Move(), allocator);
          objJson.AddMember(JsonPfbActiveKey, activeJson.Move(), allocator);
          objJson.AddMember(JsonNameKey, subNameJson.Move(), allocator);
          objJson.AddMember(JsonParentKey, parentJson.Move(), allocator);
        }
        SerializeVariantComponents(objJson, obj.m_components, allocator);

        subDataJson.PushBack(objJson.Move(), allocator);
      }
      document.AddMember(JsonPfbDataKey, subDataJson.Move(), allocator);

      // serialize removed objects
      if (!prefab.m_removedChildren.empty())
      {
        rttr::variant removedChildren{ prefab.m_removedChildren };
        rapidjson::Value removedChildrenJson{ SerializeBasedOnType(removedChildren, allocator) };
        document.AddMember(JsonRemovedChildrenKey, removedChildrenJson.Move(), allocator);
      }

      if (!prefab.m_removedComponents.empty())
      {
        rttr::variant removedComp{ prefab.m_removedComponents };
        rapidjson::Value removedCompJson{ SerializeBasedOnType(removedComp, allocator) };
        document.AddMember(JsonRemovedCompKey, removedCompJson.Move(), allocator);
      }

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
      rapidjson::Value entityName, jsonId, jsonState, jsonParent{ rapidjson::kNullType };
      rapidjson::Value childrenArr{ rapidjson::kArrayType }, compArr{ rapidjson::kArrayType };

      // serialize name
      entityName.SetString(ecs.GetEntityName(id).c_str(), allocator);
      entity.AddMember(JsonNameKey, entityName.Move(), allocator);

      // serialize entity id
      jsonId.SetUint(id);
      entity.AddMember(JsonIdKey, jsonId.Move(), allocator);

#ifndef IMGUI_DISABLE
      // serialize prefab created from
      Prefabs::PrefabManager& pm{ Prefabs::PrefabManager::GetInstance() };
      
      rapidjson::Value prefabJson{ rapidjson::kNullType };
      auto const entityPrefab{ pm.GetEntityPrefab(id) };
      if (entityPrefab)
      {
        prefabJson = SerializeBasedOnType(*entityPrefab, allocator);
      }
      entity.AddMember(JsonPrefabKey, prefabJson, allocator);
#endif

      // serialize state
      jsonState.SetBool(ecs.GetIsActiveEntity(id));
      entity.AddMember(JsonEntityStateKey, jsonState.Move(), allocator);

      // serialize parent id
      ECS::Entity const parentID{ ecs.GetParentEntity(id) };
      if (parentID != ECS::INVALID_ID)
      {
        jsonParent.SetUint(parentID);
      }
      entity.AddMember(JsonParentKey, jsonParent.Move(), allocator);

      // serialize child entities
      for (ECS::Entity const& child : ecs.GetChildEntities(id))
      {
        rapidjson::Value childJson{};
        childJson.SetUint(child);
        childrenArr.PushBack(childJson.Move(), allocator);
      }
      entity.AddMember(JsonChildEntitiesKey, childrenArr.Move(), allocator);

      std::vector<rttr::variant> const components{ ObjectFactory::ObjectFactory::GetInstance().GetEntityComponents(id) };
      for (rttr::variant const& c : components)
      {
        rapidjson::Value comp{ SerializeComponent(c, allocator) };
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

    void Serializer::SerializeVariantComponents(rapidjson::Value& value, std::vector<rttr::variant> const& components, rapidjson::Document::AllocatorType& allocator)
    {
      rapidjson::Value compArray{ rapidjson::kArrayType };
      // for each component, extract the string of the class and serialize
      for (rttr::variant const& comp : components)
      {
        rapidjson::Value compName, compJson{ rapidjson::kObjectType };
        compName.SetString(comp.get_type().get_wrapped_type().get_raw_type().get_name().to_string().c_str(), allocator);

        // handle SpriteAnim component: Get animation sprite name
        if (comp.get_type().get_wrapped_type() == rttr::type::get<Component::SpriteAnim*>())
        {
          Component::SpriteAnim const& sprAnim{ *comp.get_value<Component::SpriteAnim*>() };
          rapidjson::Value key, val, obj{ rapidjson::kObjectType };

          key.SetString("name", allocator);
          val.SetString(Graphics::GraphicsEngine::GetInstance().animManager.GetAnimName(sprAnim.animID).c_str(), allocator);
          obj.AddMember(key.Move(), val.Move(), allocator);
          compJson.AddMember(compName.Move(), obj, allocator);
        }
        else
        {
          compJson.AddMember(compName.Move(), SerializeBasedOnType(comp, allocator), allocator);
        }
        if (!compJson.ObjectEmpty())
          compArray.PushBack(compJson, allocator);
      }
      value.AddMember(JsonComponentsKey, compArray.Move(), allocator);
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
      else if (valueType == rttr::type::get<rttr::type>())
      {
        jsonVal.SetString(value.to_string().c_str(), allocator);
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
      rapidjson::Value compName, comp{ rapidjson::kObjectType }, compInner{ rapidjson::kObjectType };

      // extract the raw type of the object
      rttr::type const varType{ var.get_type().is_wrapper() ? var.get_type().get_wrapped_type().get_raw_type() :
        var.get_type().is_pointer() ? var.get_type().get_raw_type() : var.get_type() };
      compName.SetString(varType.get_name().to_string().c_str(), allocator);

      if (varType == rttr::type::get<Component::SpriteAnim>())
      {
        rapidjson::Value jsonAnimVal;
        Component::SpriteAnim const& sprAnim{ *var.get_value<Component::SpriteAnim*>() };
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
          jsonEntry.AddMember(JsonAssociativeValue, jsonValue, allocator);
          // else serialize element normally
          jsonArray.PushBack(jsonEntry.Move(), allocator);
        }
      }
    }
  } // namespace Serialization
} // namespace GE
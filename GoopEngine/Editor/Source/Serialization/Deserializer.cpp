/*!*********************************************************************
\file   Deserializer.cpp
\author chengen.lau\@digipen.edu
\date   1-November-2023
\brief  The class encapsulating functions related to deserialization.
        Currently only contains functions for the node tree and prefab
        for the prefab editor.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "Deserializer.h"
#include <rapidjson/IStreamWrapper.h>
#include <rapidjson/prettywriter.h>
#include <Component/Components.h>
#include <rttr/enumeration.h>
#include <stdarg.h>
#include <AssetManager/AssetManager.h>
#include <ScriptEngine/ScriptManager.h>
#include <Events/AnimEventManager.h>
#include <Serialization/JsonKeys.h>
#include <Prefabs/PrefabManager.h>

#ifdef _DEBUG
//#define DESERIALIZER_DEBUG
#endif

using namespace GE;
using namespace Serialization;

#ifdef _DEBUG
std::ostream& operator<<(std::ostream& os, rttr::type const& type)
{
  os << type.get_name().to_string();

  return os;
}
#endif

Prefabs::VariantPrefab Deserializer::DeserializePrefabToVariant(std::string const& json)
{
  rapidjson::Document document{};
  if (!ParseJsonIntoDocument(document, json))
  {
    return {};
  }

  if (!ScanJsonFileForMembers(document, json, 4, JsonNameKey, rapidjson::kStringType, JsonPfbDataKey, rapidjson::kArrayType,
    JsonComponentsKey, rapidjson::kArrayType, JsonPfbVerKey, rapidjson::kNumberType))
  {
    return {};
  }

  Prefabs::VariantPrefab prefab{ document[JsonNameKey].GetString(), document[JsonPfbVerKey].GetUint() };
  prefab.m_isActive = (document.HasMember(JsonPfbActiveKey) ? document[JsonPfbActiveKey].GetBool() : true);

  // iterate through component objects in json array
  std::vector<rttr::variant>& compVector{ prefab.m_components };
  for (auto const& elem : document[JsonComponentsKey].GetArray())
  {
    rapidjson::Value::ConstMemberIterator comp{ elem.MemberBegin() };
    std::string const compName{ comp->name.GetString() };
    rapidjson::Value const& compJson{ comp->value };

    rttr::type compType = rttr::type::get_by_name(compName);
#ifdef DESERIALIZER_DEBUG
    std::cout << "  [P] Deserializing " << compType << "\n";
#endif
    if (!compType.is_valid())
    {
      std::ostringstream oss{};
      oss << "Trying to deserialize an invalid component: " << compName;
      Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
      std::cout << oss.str() << "\n";
#endif
      continue;
    }

    rttr::variant compVar{};
    DeserializeComponent(compVar, compType, compJson);

    compVector.emplace_back(compVar);
  }

  for (auto const& elem : document[JsonPfbDataKey].GetArray())
  {
    if (!ScanJsonFileForMembers(elem, json, 4, JsonIdKey, rapidjson::kNumberType, JsonNameKey, rapidjson::kStringType,
      JsonComponentsKey, rapidjson::kArrayType, JsonParentKey, rapidjson::kNumberType))
    {
      continue;
    }

    Prefabs::PrefabSubData subObj{ elem[JsonNameKey].GetString(), elem[JsonIdKey].GetUint(), elem[JsonParentKey].GetUint() };
    subObj.m_isActive = (elem.HasMember(JsonPfbActiveKey) ? elem[JsonPfbActiveKey].GetBool() : true);

    for (auto const& component : elem[JsonComponentsKey].GetArray())
    {
      rapidjson::Value::ConstMemberIterator comp{ component.MemberBegin() };
      std::string const compName{ comp->name.GetString() };
      rapidjson::Value const& compJson{ comp->value };

      rttr::type compType = rttr::type::get_by_name(compName);
#ifdef DESERIALIZER_DEBUG
      std::cout << "  [P] Deserializing " << compType << "\n";
#endif
      if (!compType.is_valid())
      {
        std::ostringstream oss{};
        oss << "Trying to deserialize an invalid component: " << compName;
        Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
        std::cout << oss.str() << "\n";
#endif
        continue;
      }

      rttr::variant compVar{};
      DeserializeComponent(compVar, compType, compJson);

      subObj.AddComponent(std::move(compVar));
    }

    prefab.m_objects.emplace_back(std::move(subObj));
  }

#ifndef IMGUI_DISABLE
  if (document.HasMember(JsonRemovedChildrenKey))
  {
    rttr::variant removedChildrenVar{ std::vector<std::pair<Prefabs::PrefabSubData::SubDataId, Prefabs::PrefabVersion>>{} };
    DeserializeBasedOnType(removedChildrenVar, document[JsonRemovedChildrenKey]);
    if (removedChildrenVar.is_valid())
    {
      prefab.m_removedChildren = std::move(removedChildrenVar.get_value<
        std::vector<std::pair<Prefabs::PrefabSubData::SubDataId, Prefabs::PrefabVersion>>
      >());
    }
    else
    {
      std::string const msg{ "Unable to deserialize m_removedChildren of prefab " + prefab.m_name };
      Debug::ErrorLogger::GetInstance().LogError(msg);
#ifdef _DEBUG
      std::cout << msg << "\n";
#endif
    }
  }

  if (document.HasMember(JsonRemovedCompKey))
  {
    rttr::variant removedCompVar{ std::vector<Prefabs::VariantPrefab::RemovedComponent>{} };
    DeserializeBasedOnType(removedCompVar, document[JsonRemovedCompKey]);
    if (removedCompVar.is_valid())
    {
      prefab.m_removedComponents = std::move(removedCompVar.get_value<std::vector<Prefabs::VariantPrefab::RemovedComponent>>());
    }
    else
    {
      std::string const msg{ "Unable to deserialize m_removedComponents of prefab " + prefab.m_name };
      Debug::ErrorLogger::GetInstance().LogError(msg);
#ifdef _DEBUG
      std::cout << msg << "\n";
#endif
    }
  }
#endif

  return prefab;
}

ObjectFactory::ObjectFactory::EntityDataContainer Deserializer::DeserializeScene(std::string const& filepath)
{
  rapidjson::Document document;
  // some initial sanity checks
  if (!ParseJsonIntoDocument(document, filepath))
  {
    return{};
  }

  if (!document.IsArray())
  {
    GE::Debug::ErrorLogger::GetInstance().LogError(filepath + ": root is not an array!");
#ifdef _DEBUG
    std::cout << filepath + ": root is not an array!" << "\n";
#endif
    return {};
  }

  ObjectFactory::ObjectFactory::EntityDataContainer ret{};

  // check if scn file contains all basic keys
  if (!ScanJsonFileForMembers(document, filepath, 6,
    JsonNameKey, rapidjson::kStringType, JsonChildEntitiesKey, rapidjson::kArrayType,
    JsonIdKey, rapidjson::kNumberType, JsonParentKey, rapidjson::kNumberType,
    JsonComponentsKey, rapidjson::kArrayType, JsonEntityStateKey, rapidjson::kFalseType))
  {
    return {};
  }

  // okay code starts here
  for (auto const& entity : document.GetArray())
  {
    ECS::Entity const entityId{ entity[JsonIdKey].GetUint() };
    ECS::Entity const parentId{ entity[JsonParentKey].IsNull() ? ECS::INVALID_ID : entity[JsonParentKey].GetUint() };
    ObjectFactory::VariantEntity entityVar{ entity[JsonNameKey].GetString(),
      parentId, entity[JsonEntityStateKey].GetBool() };  // set parent
    // get child ids
    for (auto const& child : entity[JsonChildEntitiesKey].GetArray())
    {
      entityVar.m_childEntities.emplace_back(child.GetUint());
    }

#ifndef IMGUI_DISABLE
    Prefabs::PrefabManager& pm{ Prefabs::PrefabManager::GetInstance() };
    if (entity.HasMember(JsonPrefabKey) && !entity[JsonPrefabKey].IsNull())
    {
      rttr::variant mappedData{ Prefabs::VariantPrefab::EntityMappings{} };
      DeserializeBasedOnType(mappedData, entity[JsonPrefabKey]);
      pm.AttachPrefab(entityId, std::move(mappedData.get_value<Prefabs::VariantPrefab::EntityMappings>()));
    }
#endif

    // restore components
    std::vector<rttr::variant>& compVector{ entityVar.m_components };
    for (auto const& elem : entity[JsonComponentsKey].GetArray())
    {
      for (rapidjson::Value::ConstMemberIterator comp{ elem.MemberBegin() }; comp != elem.MemberEnd(); ++comp)
      {
        std::string const compName{ comp->name.GetString() };
        rapidjson::Value const& compJson{ comp->value };

        rttr::type compType = rttr::type::get_by_name(compName);
#ifdef DESERIALIZER_DEBUG
        std::cout << "  [S] Deserializing " << compType << "\n";
#endif
        if (!compType.is_valid())
        {
          std::ostringstream oss{};
          oss << "Trying to deserialize an invalid component: " << compName;
          Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
          std::cout << oss.str() << "\n";
#endif
          continue;
        }

        rttr::variant compVar{};
        DeserializeComponent(compVar, compType, compJson);

        compVector.emplace_back(std::move(compVar));
      }
    }

    ret.emplace_back(std::make_pair(entityId, std::move(entityVar)));
  }

  return ret;
}

//bool Deserializer::InvokeConstructor(rttr::variant& var, rttr::type const& varType, rapidjson::Value const& value)
//{
//  auto const ctors{ varType.get_constructors() };
//  if (ctors.empty())
//  {
//#ifdef DESERIALIZER_DEBUG
//    std::cout << "  No ctors found. Deserializing normally\n";
//#endif
//    return false;
//  }
//
//  rttr::constructor const& ctor{ *ctors.begin() };
//  if (ctor.get_parameter_infos().empty())  // if default ctr
//  {
//    var = ctor.invoke();
//#ifdef DESERIALIZER_DEBUG
//    std::cout << "  Invoking default ctor...\n";
//#endif
//    DeserializeBasedOnType(var, value);
//  }
//  else
//  {
//#ifdef DESERIALIZER_DEBUG
//    std::cout << "  Invoking non-default ctor...\n";
//#endif
//
//    std::vector<rttr::argument> args{};
//    auto const properties{ varType.get_properties() };
//    args.reserve(ctor.get_parameter_infos().size());
//    for (auto const& param : ctor.get_parameter_infos())
//    {
//      for (auto& prop : properties)
//      {
//        if (param.get_name() == prop.get_name())
//        {
//#ifdef DESERIALIZER_DEBUG
//          std::cout << "    Extracting property: " << prop.get_name().to_string() << " of type: " << prop.get_type() << "\n";
//#endif
//          rapidjson::Value::ConstMemberIterator iter{ value.FindMember(prop.get_name().to_string().c_str()) };
//          if (iter == value.MemberEnd())
//          {
//            std::ostringstream oss{};
//            oss << "Unable to find " << prop.get_name().to_string()
//              << " property in " << varType.get_name().to_string();
//            GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
//            continue;
//          }
//
//          rttr::type const propType{ prop.get_type() };
//          args.emplace_back(DeserializeElement(propType, iter->value));
//#ifdef DESERIALIZER_DEBUG
//          std::cout << "    Added " << param.get_name() << " of type " << args.back().get_type() << " to args list\n";
//#endif
//          break;
//        }
//      }
//    }
//
//    var = ctor.invoke_variadic(args);
//  }
//#ifdef DESERIALIZER_DEBUG
//    std::cout << "    Invoked ctor, returning " << varType << "\n";
//#endif
//
//  return true;
//}

void Deserializer::DeserializeClassTypes(rttr::instance objInst, rapidjson::Value const& value)
{
  rttr::instance object
  {
    objInst.get_type().get_raw_type().is_wrapper() ? objInst.get_wrapped_instance() : objInst
  };

#ifdef DESERIALIZER_DEBUG
  std::cout << "  Original instance: " << objInst.get_type() << "\n";
  std::cout << "  Extracted instance: " << object.get_type() << "\n";
#endif

  auto const properties{ object.get_type().get_properties() }; // list of properties (data members of a class)
  for (auto& prop : properties)
  {
#ifdef DESERIALIZER_DEBUG
    std::cout << "    Extracting property: " << prop.get_name().to_string() << "\n";
#endif
    // extract value based on property name
    rapidjson::Value::ConstMemberIterator iter{ value.FindMember(prop.get_name().data()) };
    if (iter == value.MemberEnd())
    {
      std::ostringstream oss{};
//      oss << "Unable to find " << prop.get_name().to_string()
//        << " property in " << object.get_type().get_name().to_string();
//#ifdef _DEBUG
//      std::cout << oss.str() << "\n";
//#endif
      continue;
    }

    rapidjson::Value const& jsonVal{ iter->value };
    switch (jsonVal.GetType())
    {
    case rapidjson::kObjectType:
    {
      rttr::variant ret{ prop.get_value(object) };
      DeserializeBasedOnType(ret, jsonVal);
      prop.set_value(object, ret);
      break;
    }
    case rapidjson::kArrayType:
    {
      rttr::variant ret{};
      if (prop.get_type().is_sequential_container())
      {
        ret = prop.get_value(object);
        rttr::variant_sequential_view view{ ret.create_sequential_view() };
        DeserializeSequentialContainer(view, jsonVal);
#ifdef DESERIALIZER_DEBUG
        if (prop.get_type() == rttr::type::get<std::vector<int>>()) {
          std::vector<int> const& vec = prop.get_value(objInst).get_value<std::vector<int>>();
          for (auto const& j : vec) {
            std::cout << j << " ";
          }
        }
        std::cout << "\n";
#endif
      }
      else if (prop.get_type().is_associative_container())
      {
        ret = prop.get_value(object);
        rttr::variant_associative_view view{ ret.create_associative_view() };
        DeserializeAssociativeContainer(view, jsonVal);
      }

      prop.set_value(object, ret);

      break;
    }
    default:
    {
      rttr::variant ret{ DeserializeBasicTypes(jsonVal) };
      // attempt to convert to respective property type
      if (ret.convert(prop.get_type()))
      {
        prop.set_value(object, ret);
      }
      else
      {
        std::ostringstream oss{};
        oss << prop.get_name().to_string() << ": Unable to convert " << ret.get_type().get_name().to_string() << " to " << prop.get_type().get_name().to_string();
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      }
      break;
    }
    }
  }
}

void Deserializer::DeserializeBasedOnType(rttr::variant& object, rapidjson::Value const& value)
{
#ifdef DESERIALIZER_DEBUG
  std::cout << "DeserializeBasedOnType: " << object.get_type() << "\n";
#endif
  switch (value.GetType())
  {
  case rapidjson::kObjectType:
  {
    if (object.get_type() == rttr::type::get<Component::ScriptInstance>())
    {
      DeserializeScriptFieldInstList(object, value);
      return;
    }
    // else if (!InvokeConstructor(object, object.get_type(), value))
    DeserializeClassTypes(object, value);
    break;
  }
  case rapidjson::kArrayType:
  {
    rttr::type const type{ object.get_type() };
    if (type.is_sequential_container())
    {
      rttr::variant_sequential_view view{ object.create_sequential_view() };
      DeserializeSequentialContainer(view, value);
    }
    else if (type.is_associative_container())
    {
      rttr::variant_associative_view view{ object.create_associative_view() };
      DeserializeAssociativeContainer(view, value);
    }

    break;
  }
  default:
    object = DeserializeBasicTypes(value);
    break;
  }
}

rttr::variant Deserializer::DeserializeElement(rttr::type const& valueType, rapidjson::Value const& value)
{
  rttr::variant ret{ DeserializeBasicTypes(value) };
  bool const result{ ret.convert(valueType) };
  if (!result)
  {
    if (value.IsObject())
    {
      rttr::constructor ctor{ valueType.get_constructor() };
      for (auto& elem : valueType.get_constructors())
      {
        if (elem.get_instantiated_type() == valueType)
        {
          ctor = elem;
        }
        ret = ctor.invoke();
        DeserializeBasedOnType(ret, value);
      }
    }
  }

  return ret;
}

void Deserializer::DeserializeSequentialContainer(rttr::variant_sequential_view& view, rapidjson::Value const& value)
{
#ifdef DESERIALIZER_DEBUG
  std::cout << "DeserializeSequentialContainer: " << view.get_type() << "\n";
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  value.Accept(writer);

  std::cout << "Json:\n" << buffer.GetString() << "\n";
#endif
  view.set_size(value.Size());  // set view size based on element count in rapidjson arr
  for (rapidjson::SizeType i{}; i < value.Size(); ++i)
  {
    auto& indexVal = value[i];
    // recursively deserialize if the element is still an array
    if (indexVal.IsArray())
    {
      auto innerView{ view.get_value(i).create_sequential_view() };
      DeserializeSequentialContainer(innerView, indexVal);
    }
    // else if key-value pair
    else if (indexVal.IsObject())
    {
      rttr::variant elem{ view.get_value(i).get_type().is_wrapper() ? view.get_value(i).extract_wrapped_value() : view.get_value(i) };
      DeserializeBasedOnType(elem, indexVal);
      if (!view.set_value(i, elem))
      {
        if (!view.set_value(i, TryDeserializeIntoInt(indexVal)))
        {
          /*std::ostringstream oss{};
          oss << "Unable to set element " << i << " of type " << elem.get_type().get_name().to_string()
            << " to container of " << view.get_type().get_name().to_string();
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());*/
        }
      }
    }
    // else deserialize normally
    else
    {
      rttr::variant elem{ DeserializeBasicTypes(indexVal) };

      if (view.get_value_type().is_enumeration())
      {
        rttr::variant enumValue = view.get_value_type().get_enumeration().name_to_value(indexVal.GetString());
        if (!view.set_value(i, enumValue))
        {
          std::ostringstream oss{};
          oss << "Unable to set element " << i << " of type " << enumValue.get_type().get_name().to_string()
            << " to container of " << view.get_type().get_name().to_string();
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        }
      }
      else if (!view.set_value(i, elem))
      {
        if (!view.set_value(i, TryDeserializeIntoInt(indexVal)))
        {
          std::ostringstream oss{};
          oss << "Unable to set element " << i << " of type " << elem.get_type().get_name().to_string()
            << " to container of " << view.get_type().get_name().to_string();
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        }
      }
    }
  }
}

void Deserializer::DeserializeAssociativeContainer(rttr::variant_associative_view& view, rapidjson::Value const& value)
{
  for (rapidjson::SizeType i{}; i < value.Size(); ++i)
  {
    rapidjson::Value const& elem{ value[i] };
    if (elem.IsObject())  // if element is key-value format
    {
      auto keyIter{ elem.FindMember(JsonAssociativeKey) }, valueIter{ elem.FindMember(JsonAssociativeValue) };
      if (keyIter == elem.MemberEnd() || valueIter == elem.MemberEnd())
      {
        std::ostringstream oss;
        oss << "Element " << i << " of " << view.get_type().get_name().to_string() << " missing "
          << JsonAssociativeKey << " or " << JsonAssociativeValue << " fields";

        continue;
      }

      rttr::variant key{ DeserializeElement(view.get_key_type(), keyIter->value) };
      rttr::variant val{};
      rttr::type const valueType{ view.get_value_type() };
      if (valueType.is_sequential_container())
      {
        if (valueType == rttr::type::get<std::vector<Component::Tween::Action>>())
        {
          val = std::vector<Component::Tween::Action>();
          DeserializeBasedOnType(val, valueIter->value);
        }
      }
      else
      {
        val = DeserializeElement(valueType, valueIter->value);
      }

      if (key && val)
      {
        view.insert(key, val);
      }
    }
    else // if its key-only
    {
      rttr::variant val{ DeserializeBasicTypes(elem) };
      if (val && val.convert(view.get_key_type()))
      {
        view.insert(val);
      }
    }
  }
}

void Deserializer::DeserializeComponent(rttr::variant& compVar, rttr::type const& compType, rapidjson::Value const& compJson)
{
#ifdef DESERIALIZER_DEBUG
  std::cout << "  Type of component variant is " << compType << "\n";
#endif

  // check for components that need to be handled differently
  if (!DeserializeOtherComponents(compVar, compType, compJson))
  {
    rttr::constructor const& compCtr{ *compType.get_constructors().begin() };
    // if ctor is default, invoke it and proceed
    if (compCtr.get_parameter_infos().empty())
    {
      compVar = compCtr.invoke();
#ifdef DESERIALIZER_DEBUG
      std::cout << "  Invoking default ctor...\n";
#endif
      DeserializeBasedOnType(compVar, compJson);
    }
    else
    {
#ifdef DESERIALIZER_DEBUG
      std::cout << "  Invoking non-default ctor...\n";
#endif

      std::vector<rttr::argument> args{}; // vector of args for ctor
      // iterate through data members of component
      auto const properties{ compType.get_properties() };
      args.reserve(compCtr.get_parameter_infos().size());
      for (auto const& param : compCtr.get_parameter_infos())
      {
        for (auto& prop : properties)
        {
          // match param name with property name
          // to get the right order of args to invoke ctor
          if (param.get_name() == prop.get_name())
          {
#ifdef DESERIALIZER_DEBUG
            std::cout << "    Extracting property: " << prop.get_name().to_string() << " of type: " << prop.get_type() << "\n";
#endif
            rapidjson::Value::ConstMemberIterator iter{ compJson.FindMember(prop.get_name().to_string().c_str()) };
            if (iter == compJson.MemberEnd())
            {
              //std::ostringstream oss{};
              //oss << "Unable to find " << prop.get_name().to_string()
              //  << " property in " << compType.get_name().to_string();
              //GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
              continue;
            }

            rttr::type const propType{ prop.get_type() };
            args.emplace_back(DeserializeElement(propType, iter->value));
#ifdef DESERIALIZER_DEBUG
            std::cout << "    Added " << param.get_name() << " of type " << args.back().get_type() << " to args list\n";
#endif
            break;
          }
        }
      }

      compVar = compCtr.invoke_variadic(args);
#ifdef DESERIALIZER_DEBUG
      std::cout << "    Invoked ctor, returning " << compVar.get_type() << "\n";
#endif
    }
  }
}

rttr::variant Deserializer::DeserializeBasicTypes(rapidjson::Value const& value)
{
  switch (value.GetType())
  {
  case rapidjson::kStringType:
    return std::string(value.GetString());
  case rapidjson::kTrueType:
  case rapidjson::kFalseType:
    return value.GetBool();
  case rapidjson::kNumberType:
    if (value.IsDouble())
      return value.GetDouble();
    else if (value.IsUint())
      return value.GetUint();
    else if (value.IsInt())
      return value.GetInt();
    else if (value.IsInt64())
      return value.GetInt64();
    else if (value.IsUint64())
      return value.GetUint64();
  default:
    return rttr::variant();
  }
}

bool Deserializer::DeserializeOtherComponents(rttr::variant& compVar, rttr::type const& type, rapidjson::Value const& value)
{
  if (type == rttr::type::get<Component::Sprite>())
  {
    if (!ScanJsonFileForMembers(value, "Sprite Component", 2, "spriteData", rapidjson::kObjectType,
      "spriteName", rapidjson::kStringType))
    {
      return true;
    }

    bool shouldRender{ (value.HasMember("shouldRender") ? value["shouldRender"].GetBool() : true) };
    rttr::variant sprDataVar{ Graphics::SpriteData() };
    DeserializeBasedOnType(sprDataVar, value["spriteData"]);
    try
    {
      compVar = type.create({ sprDataVar.get_value<Graphics::SpriteData>(), std::string(value["spriteName"].GetString()), shouldRender });
    }
    catch (Debug::IExceptionBase&)
    {
      std::ostringstream oss{};
      //oss << "Unable to load texture: " << value["spriteName"].GetString() << " | " << e.LogSource();
      oss << "Cannot find texture: " << value["spriteName"].GetString() << ". Was it deleted?";
      Debug::ErrorLogger::GetInstance().LogWarning(oss.str());
    }
    return true;
  }
  else if (type == rttr::type::get<Component::SpriteAnim>())
  {
    rapidjson::Value::ConstMemberIterator animName{ value.FindMember("name") };
    if (animName == value.MemberEnd())
    {
      GE::Debug::ErrorLogger::GetInstance().LogError("Unable to find name in SpriteAnim component");
      return true;
    }
    auto const& gEngine = Graphics::GraphicsEngine::GetInstance();
    try
    {
      Component::SpriteAnim sprAnim{ gEngine.animManager.GetAnimID(animName->value.GetString()) };
      sprAnim.flags = gEngine.animManager.GetAnim(sprAnim.animID).flags;
      compVar = std::make_shared<Component::SpriteAnim>(sprAnim);
    }
    catch (Debug::IExceptionBase& e)
    {
      std::ostringstream oss{};
      oss << "Unable to GetAnimID of " << animName->value.GetString() << " | " << e.LogSource();
      Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
      std::cout << oss.str() << "\n";
#endif
    }

    return true;
  }
  else if (type == rttr::type::get<Component::Scripts>())
  {
    compVar = rttr::type::get<ProxyScripts>().create({ value });
    return true;
  }

  return false;
}

std::vector<SpriteData> Deserializer::DeserializeSpriteSheetData(std::string const& file)
{
  rapidjson::Document document{};
  if (!ParseJsonIntoDocument(document, file))
  {
    return {};
  }

  rttr::variant ret{ std::vector<SpriteData>{} };
  DeserializeBasedOnType(ret, document);

  return ret.get_value<std::vector<SpriteData>>();
}

void Deserializer::DeserializeScriptsComponent(rttr::variant& object, std::string const& data)
{
  rapidjson::Document value;
  value.Parse(data.c_str());
  if (value.HasParseError())
  {
    Debug::ErrorLogger::GetInstance().LogError("Unable to deserialize scripts component");
    object = rttr::variant{};
    return;
  }

  // get vector of script instances
  rttr::variant scriptMap{ Component::Scripts::ScriptInstances{} };
  rapidjson::Value::ConstMemberIterator listIter{ value.FindMember("scriptList") };
  if (listIter == value.MemberEnd())
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to find \"scriptList\" property in Script component");
    object = rttr::variant{};
    return;
  }
  DeserializeBasedOnType(scriptMap, listIter->value);
  for (auto& s : scriptMap.get_value<Component::Scripts::ScriptInstances>())
  {
    s.SetAllFields();
  }
  object = rttr::type::get<Component::Scripts>().create({scriptMap.get_value<Component::Scripts::ScriptInstances>()});
}

void Deserializer::DeserializeScriptFieldInstList(rttr::variant& object, rapidjson::Value const& value)
{
  if (!ScanJsonFileForMembers(value, "ScriptFieldInstList", 3, "scriptName", rapidjson::kStringType,
    "scriptFieldInstList", rapidjson::kArrayType, "entityID", rapidjson::kNumberType))
  {
    return;
  }

  try
  {
    Component::ScriptInstance instance{ value["scriptName"].GetString(), value["entityID"].GetUint() };

    auto const& instArr{ value["scriptFieldInstList"].GetArray() };
    for (unsigned i{}; i < instance.m_scriptFieldInstList.size(); ++i)
    {
      rttr::variant& var{ instance.m_scriptFieldInstList[i] };
      for (unsigned j{}; j < instArr.Size(); ++j)
      {
        rapidjson::Value const& elem{ instArr[j] };

        if (!elem.HasMember("type") || !elem.HasMember("scriptField") || !elem["scriptField"].IsObject())
        {
          std::ostringstream oss{}; oss << "Element " << i << " of scriptFieldInstList missing fields";
          Debug::ErrorLogger::GetInstance().LogError(oss.str()); object = {}; return;
        }

        rapidjson::Value const& jsonField{ elem["scriptField"] };
        if (!jsonField.HasMember("fieldName") || !jsonField["fieldName"].IsString())
        {
          std::ostringstream oss{}; oss << "Element " << i << " of scriptFieldInstList missing fieldName in its scriptField";
          Debug::ErrorLogger::GetInstance().LogError(oss.str()); object = {}; return;
        }

        // if types or fieldNames don't match, skip to next elem
        rttr::variant scriptFieldVar{ var.get_type().get_property("scriptField").get_value(var) };
#ifdef DESERIALIZER_DEBUG
        std::cout << "    Comparing " << elem["type"].GetString() << " with " << var.get_type().get_property("type").get_value(var).get_value<std::string>()
          << "\nComparing " << jsonField["fieldName"].GetString() << " with " << scriptFieldVar.get_type().get_property("fieldName").get_value(scriptFieldVar).get_value<std::string>();
#endif
        if (elem["type"].GetString() != var.get_type().get_property("type").get_value(var).get_value<std::string>()
          || jsonField["fieldName"].GetString() != scriptFieldVar.get_type().get_property("fieldName").get_value(scriptFieldVar).get_value<std::string>())
        {
          continue;
        }

        DeserializeBasedOnType(var, elem);
      }
    }
    object = instance;
  }
  catch (Debug::IExceptionBase& e)
  {
    Debug::ErrorLogger::GetInstance().LogError(e.LogSource());
#ifdef _DEBUG
    std::cout << e.LogSource() << "\n";
#endif
    object = {};
    return;
  }
}

std::vector<std::pair<std::string, std::vector<rttr::type>>> Deserializer::DeserializeSystems(std::string const& json)
{
  rapidjson::Document document{};
  if (!ParseJsonIntoDocument(document, json))
  {
    return {};
  }

  // check if root is object
  if (!document.IsObject())
  {
    GE::Debug::ErrorLogger::GetInstance().LogError(json + ": root is not an object");
#ifdef _DEBUG
    std::cout << json << ": root is not an object" << "\n";
#endif
    return {};
  }

  std::vector<std::pair<std::string, std::vector<rttr::type>>> ret;
  // iterate through systems with key-array pairs
  for (auto const& system : document.GetObj())
  {
    std::vector<rttr::type> components{};
    for (auto const& component : system.value.GetArray())
    {
      // set component's corresponding bit
      rttr::type compType{ rttr::type::get_by_name(component.GetString()) };
      if (!compType.is_valid())
      {
        std::ostringstream oss{};
        oss << "Invalid component read in " << json << ": " << component.GetString();
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
        std::cout << oss.str() << "\n";
#endif
        continue;
      }

      components.emplace_back(std::move(compType));
    }
    ret.emplace_back(system.name.GetString(), std::move(components)); // add entry to container
  }

  return ret;
}

std::vector<AI::TreeTemplate> Deserializer::DeserializeTrees(std::string const& filepath)
{
  rapidjson::Document document{};
  if (!ParseJsonIntoDocument(document, filepath))
  {
    return {};
  }
  if (!document.IsArray())
  {
    Debug::ErrorLogger::GetInstance().LogError(filepath + " does not have a root array");
    return {};
  }

  std::vector<AI::TreeTemplate> ret{};
  for (rapidjson::Value const& tree : document.GetArray())
  {
    if (!tree.IsObject())
    {
      Debug::ErrorLogger::GetInstance().LogError(filepath + ": inner object is not an array");
      continue;
    }
    if (!ScanJsonFileForMembers(tree, filepath, 2, "treeName", rapidjson::kStringType, "treeTempID", rapidjson::kNumberType))
    {
      continue;
    }

    std::vector<AI::NodeTemplate> nodeList{};
    for (rapidjson::Value const& node : tree["tree"].GetArray())
    {
      if (!ScanJsonFileForMembers(node, filepath, 5, "nodeType", rapidjson::kStringType, "parentNode", rapidjson::kNumberType,
        "scriptName", rapidjson::kStringType, "pos", rapidjson::kStringType, "childrenNode", rapidjson::kArrayType))
      {
        continue;
      }

      AI::NodeTemplate nt;
      AI::NODE_TYPE nodeType{ rttr::type::get<AI::NODE_TYPE>().get_enumeration().name_to_value(
        node["nodeType"].GetString()).get_value<AI::NODE_TYPE>() };
      nt.m_nodeType = nodeType;
      nt.m_parentNode = node["parentNode"].GetUint();
      nt.m_scriptName = node["scriptName"].GetString();
      nt.m_pos << node["pos"].GetString();

      for (rapidjson::Value const& child : node["childrenNode"].GetArray())
      {
        nt.m_childrenNode.emplace_back(child.GetUint());
      }
      nodeList.emplace_back(std::move(nt));
    }
    ret.emplace_back(std::move(nodeList), tree["treeName"].GetString(), tree["treeTempID"].GetUint());
  }

  return ret;
}

Events::AnimEventManager::AnimEventsTable Deserializer::DeserializeAnimEventsTable(std::string const& filepath)
{
  rapidjson::Document document{};
  if (!ParseJsonIntoDocument(document, filepath))
  {
    return {};
  }
  if (!document.IsArray())
  {
    Debug::ErrorLogger::GetInstance().LogError(filepath + ": root is not an array");
    return {};
  }

  Events::AnimEventManager::AnimEventsTable animEventsTable{};
  for (rapidjson::Value const& element : document.GetArray())
  {
    rapidjson::Value const& subMapJson{ element[Serialization::JsonAssociativeValue] };
    if (!element.IsObject())
    {
      Debug::ErrorLogger::GetInstance().LogError(filepath + ": Element is not an object");
      continue;
    }
    else if (!subMapJson.IsArray())
    {
      Debug::ErrorLogger::GetInstance().LogError(filepath + ": Value of AnimEventsTable is not an object");
      continue;
    }

    Component::AnimEvents::AnimEventsCont container{};
    for (rapidjson::Value const& subElem : subMapJson.GetArray())
    {
      rapidjson::Value const& jsonVec{ subElem[Serialization::JsonAssociativeValue] };
      if (!subElem.IsObject())
      {
        Debug::ErrorLogger::GetInstance().LogError(filepath + ": Element of AnimEventsCont is not an object");
        continue;
      }
      else if (!jsonVec.IsArray())
      {
        Debug::ErrorLogger::GetInstance().LogError(filepath + ": Value of AnimEventsCont is not an Array");
        continue;
      }

      Component::AnimEvents::AnimFrameEvents strVec{};
      strVec.reserve(jsonVec.Size());
      for (rapidjson::Value const& jsonStr : jsonVec.GetArray())
      {
        strVec.emplace_back(jsonStr.GetString());
      }
      container.emplace(subElem[Serialization::JsonAssociativeKey].GetUint(), std::move(strVec));
    }
    animEventsTable.emplace(element[Serialization::JsonAssociativeKey].GetString(), std::move(container));
  }
  return animEventsTable;
}

Deserializer::EntityScriptsList Deserializer::DeserializeSceneScripts(std::string const& file)
{
  rapidjson::Document document{};
  if (!ParseJsonIntoDocument(document, file))
  {
    throw Debug::Exception<Deserializer>(Debug::LEVEL_ERROR, ErrMsg("Unable to parse scene file"));
  }
  if (!document.IsArray())
  {
    throw Debug::Exception<Deserializer>(Debug::LEVEL_ERROR, ErrMsg("Root is not an array"));
  }

  EntityScriptsList ret{};
  // check if scene contains all basic keys
  if (!ScanJsonFileForMembers(document, file, 2,JsonIdKey, rapidjson::kNumberType,
    JsonComponentsKey, rapidjson::kArrayType))
  {
    throw Debug::Exception<Deserializer>(Debug::LEVEL_ERROR, ErrMsg("Scene file missing fields!"));
  }

  for (auto const& entity : document.GetArray())
  {
    ECS::Entity const entityId{ entity[JsonIdKey].GetUint() };

    rttr::variant scriptInstances{ Component::Scripts::ScriptInstances{} };
    rttr::type const scriptCompType{ rttr::type::get<Component::Scripts>() };
    std::string const scriptCompName{ scriptCompType.get_name().to_string() };
    for (auto const& elem : entity[JsonComponentsKey].GetArray())
    {
      for (rapidjson::Value::ConstMemberIterator comp{ elem.MemberBegin() }; comp != elem.MemberEnd(); ++comp)
      {
        std::string const compName{ comp->name.GetString() };
        // we only care about the script component
        if (compName != scriptCompName)
        {
          continue;
        }

        rapidjson::Value const& compJson{ comp->value };
        rapidjson::Value::ConstMemberIterator listIter{ compJson.FindMember("scriptList") };
        if (listIter == compJson.MemberEnd())
        {
          throw Debug::Exception<Deserializer>(Debug::LEVEL_ERROR, ErrMsg("Unable to find \"scriptList\" property in Script component"));
        }

        DeserializeBasedOnType(scriptInstances, listIter->value);
        for (auto& s : scriptInstances.get_value<Component::Scripts::ScriptInstances>())
        {
          s.SetAllFields();
        }
      }
    }

    ret.emplace_back(entityId, std::move(scriptInstances.get_value<Component::Scripts::ScriptInstances>()));
  }

  return ret;

}

bool Deserializer::ScanJsonFileForMembers(rapidjson::Value const& value, std::string const& filename, unsigned keyCount, ...)
{
  va_list args;
  va_start(args, keyCount);

  bool status{ true };
  std::vector <std::pair<std::string, rapidjson::Type>> keys{};
  for (unsigned i{}; i < keyCount; ++i)
  {
    std::string test{ va_arg(args, const char*) };
    keys.emplace_back(std::move(test), static_cast<rapidjson::Type>(va_arg(args, int)));
  }

  if (value.IsArray())
  {
    for (unsigned i{}; i < value.GetArray().Size(); ++i)
    {
      for (auto const& [keyName, type] : keys)
      {
        rapidjson::Value const& elem{ value[i] };
        auto result{ elem.FindMember(keyName.c_str()) };
        if (result == elem.MemberEnd())
        {
          std::ostringstream oss{};
          oss << filename << ": Unable to find key \"" + keyName + "\" of element: " << i << " in rapidjson value";
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
          std::cout << oss.str() << "\n";
#endif
          status = false;
          continue;
        }

        if ((type == rapidjson::kTrueType || type == rapidjson::kFalseType))
        {
          if (!elem[keyName.c_str()].IsBool())
          {
            std::ostringstream oss{};
            oss << filename << ": Element \"" << keyName << "\" is not of type bool";
            GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
            std::cout << oss.str() << "\n";
#endif
            status = false;
          }
        }
        else if (!elem[keyName.c_str()].IsNull() && elem[keyName.c_str()].GetType() != type)
        {
          std::ostringstream oss{};
          oss << filename << ": Element \"" << keyName << "\" is not of rapidjson type:" << type;
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
          std::cout << oss.str() << "\n";
#endif
          status = false;
        }
      }
    }
  }
  else
  {
    for (auto const& [keyName, type] : keys)
    {
      auto result{ value.FindMember(keyName.c_str()) };
      if (result == value.MemberEnd())
      {
        std::ostringstream oss{};
        oss << filename << ": Unable to find key \"" << keyName << "\" in rapidjson value";
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
        std::cout << oss.str() << "\n";
#endif
        status = false;
        continue;
      }

      if ((type == rapidjson::kTrueType || type == rapidjson::kFalseType))
      {
        if (!value[keyName.c_str()].IsBool())
        {
          std::ostringstream oss{};
          oss << filename << ": Element \"" << keyName << "\" is not of type bool";
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
          std::cout << oss.str() << "\n";
#endif
          status = false;
        }
      }
      else if (!value[keyName.c_str()].IsNull() && value[keyName.c_str()].GetType() != type)
      {
        std::ostringstream oss{};
        oss << filename << ": Element \"" << keyName << "\" is not of rapidjson type:" << type;
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
        std::cout << oss.str() << "\n";
#endif
        status = false;
      }
    }
  }

  va_end(args);
  return status;
}

bool Deserializer::ParseJsonIntoDocument(rapidjson::Document& document, std::string const& filepath)
{
  std::ifstream ifs{ filepath };
  if (!ifs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + filepath);
#ifdef _DEBUG
    std::cout << "Unable to read " << filepath << "\n";
#endif
    return false;
  }
  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (ifs.peek() == std::ifstream::traits_type::eof())
  {
    ifs.close(); GE::Debug::ErrorLogger::GetInstance().LogMessage("Empty scene file read. Ignoring checks");
    return false;
  }

  if (document.ParseStream(isw).HasParseError())
  {
    ifs.close(); GE::Debug::ErrorLogger::GetInstance().LogError("Unable to parse " + filepath);
#ifdef _DEBUG
    std::cout << "Unable to parse " + filepath << "\n";
#endif
    return false;
  }

  return true;
}

rttr::variant Deserializer::TryDeserializeIntoInt(rapidjson::Value const& value)
{
  if (value.IsInt())
  {
    return value.GetInt();
  }

  return {};
}
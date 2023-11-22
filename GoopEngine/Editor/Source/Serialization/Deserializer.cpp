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
#include <Serialization/Serializer.h>
#include "Deserializer.h"
#include <rapidjson/IStreamWrapper.h>
#include <Component/Components.h>
#include <rttr/enumeration.h>
#include <ObjectFactory/SerializeComponents.h>
#include <rapidjson/prettywriter.h>
#include <stdarg.h>
#include <AssetManager/AssetManager.h>

#ifdef _DEBUG
#define DESERIALIZER_DEBUG
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

ObjectFactory::VariantPrefab Deserializer::DeserializePrefabToVariant(std::string const& json)
{
  std::ifstream ifs{ json };
  if (!ifs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + json);
    #ifdef _DEBUG
    std::cout << "Unable to read " << json << "\n";
    #endif
    return {};
  }
  rapidjson::Document document{};
  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (document.ParseStream(isw).HasParseError())
  {
    ifs.close(); GE::Debug::ErrorLogger::GetInstance().LogError("Unable to parse " + json);
    #ifdef _DEBUG
    std::cout << "Unable to parse " + json << "\n";
    #endif
    return {};
  }

  if (!ScanJsonFileForMembers(document, 2, Serializer::JsonNameKey, rapidjson::kStringType, 
    Serializer::JsonComponentsKey, rapidjson::kArrayType)) { ifs.close(); return {}; }

  ObjectFactory::VariantPrefab prefab{ document[Serializer::JsonNameKey].GetString() };
  // iterate through component objects in json array
  std::vector<rttr::variant>& compVector{ prefab.m_components };
  for (auto const& elem : document[Serializer::JsonComponentsKey].GetArray())
  {
    for (rapidjson::Value::ConstMemberIterator comp{ elem.MemberBegin() }; comp != elem.MemberEnd(); ++comp)
    {
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
  }

  return prefab;
}

ObjectFactory::ObjectFactory::EntityDataContainer Deserializer::DeserializeScene(std::string const& filepath)
{
  std::ifstream ifs{ filepath };

  // just some initial sanity checks...
  if (!ifs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + filepath);
    #ifdef _DEBUG
    std::cout << "Unable to read " << filepath << "\n";
    #endif
    return {};
  }
  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (ifs.peek() == std::ifstream::traits_type::eof())
  {
    ifs.close(); GE::Debug::ErrorLogger::GetInstance().LogError("Empty scene file read. Ignoring checks");
    return {};
  }
  rapidjson::Document document{};
  if (document.ParseStream(isw).HasParseError())
  {
    ifs.close(); GE::Debug::ErrorLogger::GetInstance().LogError("Unable to parse " + filepath);
    #ifdef _DEBUG
    std::cout << "Unable to parse " + filepath << "\n";
    #endif
    return {};
  }
  if (!document.IsArray())
  { 
    ifs.close(); GE::Debug::ErrorLogger::GetInstance().LogError(filepath + ": root is not an array!");
    #ifdef _DEBUG
    std::cout << filepath + ": root is not an array!" << "\n";
    #endif
    return {};
  }

  ObjectFactory::ObjectFactory::EntityDataContainer ret{};

  // check if scn file contains all basic keys
  if (!ScanJsonFileForMembers(document, 5,
    Serializer::JsonNameKey, rapidjson::kStringType, Serializer::JsonChildEntitiesKey, rapidjson::kArrayType,
    Serializer::JsonIdKey, rapidjson::kNumberType, Serializer::JsonParentKey, rapidjson::kNumberType,
    Serializer::JsonComponentsKey, rapidjson::kArrayType)) { ifs.close(); return {}; }

  // okay code starts here
  for (auto const& entity : document.GetArray())
  {
    ECS::Entity const parent_id{ entity[Serializer::JsonParentKey].IsNull() ? ECS::INVALID_ID : entity[Serializer::JsonParentKey].GetUint() };
    ObjectFactory::VariantEntity entityVar{ entity[Serializer::JsonNameKey].GetString(), parent_id };  // set parent
    // get child ids
    for (auto const& child : entity[Serializer::JsonChildEntitiesKey].GetArray())
    {
      entityVar.m_childEntities.emplace_back(child.GetUint());
    }

    std::vector<rttr::variant>& compVector{ entityVar.m_components };
    for (auto const& elem : entity[Serializer::JsonComponentsKey].GetArray())
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

    ret.emplace_back(std::make_pair(entity[Serializer::JsonIdKey].GetUint(), std::move(entityVar)));
  }

  return ret;
}

void Deserializer::DeserializeBasedOnType(rttr::instance object, rapidjson::Value const& value)
{
  // extract underlying type if its a wrapper
  rttr::instance objInstance
  {
    object.get_type().get_raw_type().is_wrapper() ? object.get_wrapped_instance() : object
  };
  auto const properties{ objInstance.get_type().get_properties() }; // list of properties (data members of a class)
  #ifdef DESERIALIZER_DEBUG
  std::cout << "  Original instance: " << object.get_type() << "\n";
  std::cout << "  Extracted instance: " << objInstance.get_type() << "\n";
  #endif
  for (auto& prop : properties)
  {
    #ifdef DESERIALIZER_DEBUG
    std::cout << "    Extracting property: " << prop.get_name() << "\n";
    #endif
    // extract value based on property name
    rapidjson::Value::ConstMemberIterator iter{ value.FindMember(prop.get_name().data()) };
    if (iter == value.MemberEnd())
    {
      std::ostringstream oss{};
      oss << "Unable to find " << prop.get_name().to_string()
        << " property in " << objInstance.get_type().get_name().to_string();
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
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
        rttr::variant_sequential_view view = ret.create_sequential_view();
        DeserializeSequentialContainer(view, jsonVal);
      }
      else if (prop.get_type().is_associative_container())
      {
        GE::Debug::ErrorLogger::GetInstance().LogMessage(
          prop.get_name().to_string() + ": Associative Containers are not yet supported");
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
        prop.set_value(objInstance, ret);
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

rttr::variant Deserializer::DeserializeElement(rttr::type const& valueType, rapidjson::Value const& value)
{
  rttr::variant ret{ DeserializeBasicTypes(value) };
  bool const result{ ret.convert(valueType) };
  if (!result)
  {
    if (!value.IsObject())
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
      rttr::variant elem{ view.get_value(i).extract_wrapped_value() };
      DeserializeBasedOnType(elem, indexVal);
      view.set_value(i, elem);
    }
    // else deserialize normally
    else
    {  
      rttr::type const arrType{ view.get_rank_type(i) };
      rttr::variant elem{ DeserializeBasicTypes(indexVal) };
      
      view.set_value(i, elem);
    }
  }
}

void Deserializer::DeserializeComponent(rttr::variant& compVar, rttr::type const& compType, rapidjson::Value const& compJson)
{
  // check for components that need to be handled differently
  if (!DeserializeOtherComponents(compVar, compType, compJson))
  {
    rttr::constructor const& compCtr{ *compType.get_constructors().begin() };
    if (compCtr.get_parameter_infos().empty())  // if default ctr
    {
      compVar = compCtr.invoke();
      #ifdef DESERIALIZER_DEBUG
      std::cout << "  Default ctr invoked...Type of compVar is " << compVar.get_type() << "\n";
      #endif
      DeserializeBasedOnType(compVar, compJson);
    }
    else
    {
      #ifdef DESERIALIZER_DEBUG
      std::cout << "  Non-default ctr invoked\n";
      #endif
      rttr::instance object{ compVar };
      rttr::instance objInstance
      {
        object.get_type().get_raw_type().is_wrapper() ? object.get_wrapped_instance() : object
      };

      std::vector<rttr::variant> args{};
      auto const properties{ objInstance.get_type().get_properties() };
      args.reserve(compCtr.get_parameter_infos().size());
      for (auto const& param : compCtr.get_parameter_infos())
      {
        for (auto& prop : properties)
        {
          if (param.get_name() == prop.get_name())
          {
            args.emplace_back(prop.get_value(object));
            break;
          }
        }
      }

      compVar = compCtr.invoke(args);
    }
  }
}

bool Deserializer::DeserializeOtherComponents(rttr::variant& compVar, rttr::type const& type, rapidjson::Value const& value)
{
  if (type == rttr::type::get<Component::Sprite>())
  {
    auto const& gEngine{ Graphics::GraphicsEngine::GetInstance() };
    auto& am{ GE::Assets::AssetManager::GetInstance() };

    rapidjson::Value::ConstMemberIterator iter{ value.FindMember(type.get_properties().begin()->get_name().to_string().c_str()) };
    if (iter == value.MemberEnd())
    {
      std::ostringstream oss{};
      oss << "Unable to find " << type.get_properties().begin()->get_name().to_string().c_str()
        << " property in " << type.get_name().to_string();
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return false;
    }

    try { am.GetData(iter->value.GetString()); }
    catch (GE::Debug::IExceptionBase&) { am.LoadImageW(iter->value.GetString()); }

    compVar = type.create({ gEngine.textureManager.GetTextureID(iter->value.GetString()) });
    return true;
  }
  else if (type == rttr::type::get<Component::SpriteAnim>())
  {
    rapidjson::Value::ConstMemberIterator iter{ value.FindMember(type.get_properties().begin()->get_name().to_string().c_str()) };
    if (iter == value.MemberEnd())
    {
      std::ostringstream oss{};
      oss << "Unable to find " << type.get_properties().begin()->get_name().to_string().c_str()
        << " property in " << type.get_name().to_string();
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return false;
    }

    compVar = type.create({ Graphics::GraphicsEngine::GetInstance().animManager.GetAnimID(iter->value.GetString()) });
    return true;
  }
  else if (type == rttr::type::get<Component::Scripts>())
  {
    rttr::variant scriptMap{ std::vector<std::string>{} };
    std::vector<rttr::argument> args{};
    args.reserve(2);
    try
    {
      rttr::variant_sequential_view view = scriptMap.create_sequential_view();
      DeserializeSequentialContainer(view, value["scriptMap"]);
      args.emplace_back(std::move(scriptMap));
      args.emplace_back(value["entityId"].GetUint());
    }
    catch (...)
    {
      std::ostringstream oss{};
      oss << "Unable to deserialize " << type.get_name().to_string();
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return false;
    }
    
    compVar = type.create(std::move(args));
    return true;
  }

  return false;
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

std::vector<AI::TreeTemplate> Deserializer::DeserializeTrees(std::string const& filename)
{
  std::ifstream ifs{ filename };
  if (!ifs)
  {
    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to read " + filename));
  }
  rapidjson::Document document{};
  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (document.ParseStream(isw).HasParseError())
  {
    ifs.close();
    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to parse " + filename));
  }
  if (!document.IsArray())
  {
    ifs.close();
    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg(filename + " does not have a root array"));
  }

  std::vector<AI::TreeTemplate> ret{};
  for (rapidjson::Value const& tree : document.GetArray())
  {
    if (!tree.IsObject())
    {
      ifs.close();
      throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg(filename + ": inner object is not an array"));
    }

    std::vector<AI::NodeTemplate> nodeList{};
    for (rapidjson::Value const& node : tree["tree"].GetArray())
    {
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

std::vector<std::pair<std::string, ECS::ComponentSignature>> Deserializer::DeserializeSystems(std::string const& json)
{
  std::ifstream ifs{ json };
  if (!ifs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + json);
    #ifdef _DEBUG
    std::cout << "Unable to read " << json << "\n";
    #endif
    return {};
  }

  rapidjson::Document document{};
  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (document.ParseStream(isw).HasParseError())
  {
    ifs.close();
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to parse " + json);
    #ifdef _DEBUG
    std::cout << "Unable to parse " << json << "\n";
    #endif
    return {};
  }

  // check if root is object
  if (!document.IsObject())
  {
    ifs.close();
    GE::Debug::ErrorLogger::GetInstance().LogError(json + ": root is not an object");
    #ifdef _DEBUG
    std::cout << json << ": root is not an object" << "\n";
    #endif
    return {};
  }

  std::vector<std::pair<std::string, ECS::ComponentSignature>> ret;
  // iterate through systems with key-array pairs
  for (auto const& system : document.GetObj())
  {
    ECS::ComponentSignature sig{};
    for (auto const& component : system.value.GetArray())
    {
      // set component's corresponding bit
      auto compType{ ECS::stringToComponents.find(component.GetString()) };
      if (compType == ECS::stringToComponents.cend())
      {
        ifs.close();
        std::ostringstream oss{};
        oss << "Invalid component read in " << json << ": " << component.GetString();
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        #ifdef _DEBUG
        std::cout << oss.str() << "\n";
        #endif
        continue;
      }

      sig[static_cast<unsigned>(compType->second)] = true;
    }
    ret.emplace_back(system.name.GetString(), sig); // add entry to container
  }

  ifs.close();
  return ret;
}

bool Deserializer::ScanJsonFileForMembers(rapidjson::Document const& document, unsigned keyCount, ...)
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

  if (document.IsArray())
  {
    for (unsigned i{}; i < document.GetArray().Size(); ++i)
    {
      for (auto const& [keyName, type] : keys)
      {
        rapidjson::Value const& elem{ document[i] };
        auto result{ elem.FindMember(keyName.c_str()) };
        if (result == elem.MemberEnd())
        {
          std::ostringstream oss{};
          oss << "Unable to find key \"" + keyName + "\" of element: " << i << " in rapidjson document";
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
          #ifdef _DEBUG
          std::cout << oss.str() << "\n";
          #endif
          status = false;
          continue;
        }

        if (!elem[keyName.c_str()].IsNull() && elem[keyName.c_str()].GetType() != type)
        {
          std::ostringstream oss{};
          oss << "Element \"" << keyName << "\" is not of rapidjson type:" << type;
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
          #ifdef _DEBUG
          std::cout << oss.str() << "\n";
          #endif
          status = false;
          continue;
        }
      }
    }
  }
  else
  {
    for (auto const& [keyName, type] : keys)
    {
      auto result{ document.FindMember(keyName.c_str()) };
      if (result == document.MemberEnd())
      {
        std::string const msg{ "Unable to find key \"" + keyName + "\" in rapidjson document" };
        GE::Debug::ErrorLogger::GetInstance().LogError(msg);
        #ifdef _DEBUG
        std::cout << msg << "\n";
        #endif
        status = false;
        continue;
      }

      if (!document[keyName.c_str()].IsNull() && document[keyName.c_str()].GetType() != type)
      {
        std::ostringstream oss{};
        oss << "Element \"" << keyName << "\" is not of rapidjson type:" << type;
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        #ifdef _DEBUG
        std::cout << oss.str() << "\n";
        #endif
        status = false;
        continue;
      }
    }
  }

  va_end(args);
  return status;
}

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
#include <rapidjson/prettywriter.h>
#include <stdarg.h>
#include <AssetManager/AssetManager.h>
#include <ScriptEngine/ScriptManager.h>

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
  if (!ScanJsonFileForMembers(document, 6,
    Serializer::JsonNameKey, rapidjson::kStringType, Serializer::JsonChildEntitiesKey, rapidjson::kArrayType,
    Serializer::JsonIdKey, rapidjson::kNumberType, Serializer::JsonParentKey, rapidjson::kNumberType,
    Serializer::JsonComponentsKey, rapidjson::kArrayType, Serializer::JsonEntityStateKey, rapidjson::kFalseType)) { ifs.close(); return {}; }

  // okay code starts here
  for (auto const& entity : document.GetArray())
  {
    ECS::Entity const parent_id{ entity[Serializer::JsonParentKey].IsNull() ? ECS::INVALID_ID : entity[Serializer::JsonParentKey].GetUint() };
    ObjectFactory::VariantEntity entityVar{ entity[Serializer::JsonNameKey].GetString(),
      parent_id, entity[Serializer::JsonEntityStateKey].GetBool() };  // set parent
    // get child ids
    for (auto const& child : entity[Serializer::JsonChildEntitiesKey].GetArray())
    {
      entityVar.m_childEntities.emplace_back(child.GetUint());
    }

    // restore components
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

bool Deserializer::InvokeConstructor(rttr::variant& var, rttr::type const& varType, rapidjson::Value const& value)
{
  auto const ctors{ varType.get_constructors() };
  if (ctors.empty())
  {
#ifdef DESERIALIZER_DEBUG
    std::cout << "  No ctors found. Deserializing normally\n";
#endif
    return false;
  }

  rttr::constructor const& ctor{ *ctors.begin() };
  if (ctor.get_parameter_infos().empty())  // if default ctr
  {
    var = ctor.invoke();
#ifdef DESERIALIZER_DEBUG
    std::cout << "  Invoking default ctor...\n";
#endif
    DeserializeBasedOnType(var, value);
  }
  else
  {
#ifdef DESERIALIZER_DEBUG
    std::cout << "  Invoking non-default ctor...\n";
#endif

    std::vector<rttr::argument> args{};
    auto const properties{ varType.get_properties() };
    args.reserve(ctor.get_parameter_infos().size());
    for (auto const& param : ctor.get_parameter_infos())
    {
      for (auto& prop : properties)
      {
        if (param.get_name() == prop.get_name())
        {
#ifdef DESERIALIZER_DEBUG
          std::cout << "    Extracting property: " << prop.get_name().to_string() << " of type: " << prop.get_type() << "\n";
#endif
          rapidjson::Value::ConstMemberIterator iter{ value.FindMember(prop.get_name().to_string().c_str()) };
          if (iter == value.MemberEnd())
          {
            std::ostringstream oss{};
            oss << "Unable to find " << prop.get_name().to_string()
              << " property in " << varType.get_name().to_string();
            GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
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

    var = ctor.invoke_variadic(args);
  }
#ifdef DESERIALIZER_DEBUG
    std::cout << "    Invoked ctor, returning " << varType << "\n";
#endif

  return true;
}

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
      oss << "Unable to find " << prop.get_name().to_string()
        << " property in " << object.get_type().get_name().to_string();
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
      std::cout << oss.str() << "\n";
#endif
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
        if (prop.get_name().to_string() == "scriptFieldInstList")
        {
          DeserializeScriptFieldInstList(ret, jsonVal);
        }
        else
        {
          ret = prop.get_value(object);
          rttr::variant_sequential_view view = ret.create_sequential_view();
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
      rapidjson::Value::ConstMemberIterator iter{ value.FindMember("scriptName") };
      if (iter == value.MemberEnd()) { object = {}; return; }

      object = Component::ScriptInstance(value["scriptName"].GetString());
      DeserializeClassTypes(object, value);
    }
    else if (!InvokeConstructor(object, object.get_type(), value))
      DeserializeClassTypes(object, value);
    break;
  }
  case rapidjson::kArrayType:
  {
    rttr::type const type{ object.get_type() };
    if (type.is_sequential_container())
    {
      rttr::variant_sequential_view view = object.create_sequential_view();
      DeserializeSequentialContainer(view, value);
#ifdef DESERIALIZER_DEBUG
      if (type == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<int>>>()) {
        GE::MONO::ScriptFieldInstance<std::vector<int>> const& vec = object.get_value< GE::MONO::ScriptFieldInstance<std::vector<int>>>();
        for (auto const& j : vec.m_data) {
          std::cout << j << " ";
        }
      }
      std::cout << "\n";
#endif
    }
    else if (type.is_associative_container())
    {
      GE::Debug::ErrorLogger::GetInstance().LogMessage(
        type.get_name().to_string() + ": Associative Containers are not yet supported");
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
      rttr::variant elem{ view.get_value(i).extract_wrapped_value() };
      DeserializeBasedOnType(elem, indexVal);
      if (!view.set_value(i, elem))
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
    // else deserialize normally
    else
    {  
      rttr::type const arrType{ view.get_rank_type(i) };
      rttr::variant elem{ DeserializeBasicTypes(indexVal) };
      
      if (!view.set_value(i, elem))
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

void Deserializer::DeserializeComponent(rttr::variant& compVar, rttr::type const& compType, rapidjson::Value const& compJson)
{
  #ifdef DESERIALIZER_DEBUG
  std::cout <<  "  Type of component variant is " << compType << "\n";
  #endif

  // check for components that need to be handled differently
  if (!DeserializeOtherComponents(compVar, compType, compJson))
  {
    rttr::constructor const& compCtr{ *compType.get_constructors().begin() };
    if (compCtr.get_parameter_infos().empty())  // if default ctr
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

      std::vector<rttr::argument> args{};
      auto const properties{ compType.get_properties() };
      args.reserve(compCtr.get_parameter_infos().size());
      for (auto const& param : compCtr.get_parameter_infos())
      {
        for (auto& prop : properties)
        {
          if (param.get_name() == prop.get_name())
          {
#ifdef DESERIALIZER_DEBUG
            std::cout << "    Extracting property: " << prop.get_name().to_string() << " of type: " << prop.get_type() << "\n";
#endif
            rapidjson::Value::ConstMemberIterator iter{ compJson.FindMember(prop.get_name().to_string().c_str()) };
            if (iter == compJson.MemberEnd())
            {
              std::ostringstream oss{};
              oss << "Unable to find " << prop.get_name().to_string()
                << " property in " << compType.get_name().to_string();
              GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
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

bool Deserializer::DeserializeOtherComponents(rttr::variant& compVar, rttr::type const& type, rapidjson::Value const& value)
{
  if (type == rttr::type::get<Component::Sprite>())
  {
    rapidjson::Value::ConstMemberIterator sprData{ value.FindMember("spriteData") };
    if (sprData == value.MemberEnd())
    {
      GE::Debug::ErrorLogger::GetInstance().LogError("Unable to find spriteData in Sprite component");
      return false;
    }
    rapidjson::Value::ConstMemberIterator sprName{ value.FindMember("spriteName") };
    if (sprData == value.MemberEnd())
    {
      GE::Debug::ErrorLogger::GetInstance().LogError("Unable to find spriteName in Sprite component");
      return false;
    }
    
    rttr::variant sprDataVar{ Graphics::SpriteData() };
    DeserializeBasedOnType(sprDataVar, sprData->value);
    compVar = type.create({ sprDataVar.get_value<Graphics::SpriteData>(), std::string(sprName->value.GetString())});
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
      return true;
    }

    compVar = type.create({ Graphics::GraphicsEngine::GetInstance().animManager.GetAnimID(iter->value.GetString()) });
    return true;
  }
  else if (type == rttr::type::get<Component::Scripts>())
  {
    //GE::Debug::ErrorLogger::GetInstance().LogMessage("Deserializing of Script component is skipped for now");
    // get entity id
    rapidjson::Value::ConstMemberIterator idIter{ value.FindMember("entityId") };
    if (idIter == value.MemberEnd())
    {
      GE::Debug::ErrorLogger::GetInstance().LogError("Unable to find \"entityId\" property in Script component");
      return true;
    }

    // get vector of script instances
    rttr::variant scriptMap{ Component::Scripts::ScriptInstances{} };
    rapidjson::Value::ConstMemberIterator listIter{ value.FindMember("scriptList") };
    if (listIter == value.MemberEnd())
    {
      GE::Debug::ErrorLogger::GetInstance().LogError("Unable to find \"scriptList\" property in Script component");
      return true;
    }
    DeserializeBasedOnType(scriptMap, listIter->value);

    Component::Scripts::ScriptInstances const& test = scriptMap.get_value<Component::Scripts::ScriptInstances>();
    compVar = type.create({ idIter->value.GetUint(),  test});

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

void Deserializer::DeserializeScriptFieldInstList(rttr::variant& object, rapidjson::Value const& value)
{
#ifdef DESERIALIZER_DEBUG
  std::cout << "Deserializing ScriptFieldInstList...\n";
#endif
  if (!value.IsArray()) { Debug::ErrorLogger::GetInstance().LogError("scriptFieldInstList is not a rapidjson Array!"); return; }

  std::vector<rttr::variant> scriptFieldInstList{};
  scriptFieldInstList.reserve(value.Size());
  for (rapidjson::SizeType i{}; i < value.Size(); ++i)
  {
    rapidjson::Value const& elem{ value[i] };
    if (!elem.HasMember("type")) { Debug::ErrorLogger::GetInstance().LogError("ScriptInstance missing \"scriptFieldInstList\" member"); continue; }
    
    rttr::variant scriptFieldInst{ MONO::ScriptManager::GetInstance().GetScriptFieldInst(elem["type"].GetString()) };
    DeserializeBasedOnType(scriptFieldInst, elem);
#ifdef DESERIALIZER_DEBUG
    std::cout << "  Reading " << elem["type"].GetString() << "...\n";
#endif
    scriptFieldInstList.emplace_back(scriptFieldInst);
#ifdef DESERIALIZER_DEBUG
    std::cout << "  Added " << scriptFieldInst.get_type() << " to ScriptFieldInstList\n";
      if (scriptFieldInst.get_type() == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<int>>>()) {
        GE::MONO::ScriptFieldInstance<std::vector<int>> const& vec = scriptFieldInst.get_value< GE::MONO::ScriptFieldInstance<std::vector<int>>>();
        for (auto const& j : vec.m_data) {
          std::cout << j << " ";
        }
      }
    std::cout << "\n";
#endif
  }

#ifdef DESERIALIZER_DEBUG
  for (auto const& i : scriptFieldInstList)
  {
    if (i.get_type() == rttr::type::get<GE::MONO::ScriptFieldInstance<std::vector<int>>>()) {
      GE::MONO::ScriptFieldInstance<std::vector<int>> const& vec = i.get_value< GE::MONO::ScriptFieldInstance<std::vector<int>>>();
      for (auto const& j : vec.m_data) {
        std::cout << j << " ";
      }
    }
  }
  std::cout << "\n";
#endif

  object = scriptFieldInstList;
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

        if ((type == rapidjson::kTrueType || type == rapidjson::kFalseType))
        {
          if (!elem[keyName.c_str()].IsBool())
          {
            std::ostringstream oss{};
            oss << "Element \"" << keyName << "\" is not of type bool";
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
          oss << "Element \"" << keyName << "\" is not of rapidjson type:" << type;
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

      if ((type == rapidjson::kTrueType || type == rapidjson::kFalseType))
      {
        if (!document[keyName.c_str()].IsBool())
        {
          std::ostringstream oss{};
          oss << "Element \"" << keyName << "\" is not of type bool";
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
#ifdef _DEBUG
          std::cout << oss.str() << "\n";
#endif
          status = false;
        }
      }
      else if (!document[keyName.c_str()].IsNull() && document[keyName.c_str()].GetType() != type)
      {
        std::ostringstream oss{};
        oss << "Element \"" << keyName << "\" is not of rapidjson type:" << type;
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


rttr::variant Deserializer::TryDeserializeIntoInt(rapidjson::Value const& value)
{
  if (value.IsInt())
  {
    return value.GetInt();
  }

  return {};
}
#include <pch.h>
#include <Serialization/Serializer.h>
#include "Deserializer.h"
#include <rapidjson/IStreamWrapper.h>
#include <Component/Components.h>
#include <rttr/enumeration.h>

using namespace GE;
using namespace Serialization;

//ObjectFactory::VariantPrefab Deserializer::DeserializePrefab(std::string const& json)
//{
//  std::ifstream ifs{ json };
//  if (!ifs)
//  {
//    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to read " + json));
//  }
//  rapidjson::Document document{};
//  // parse into document object
//  rapidjson::IStreamWrapper isw{ ifs };
//  if (document.ParseStream(isw).HasParseError())
//  {
//    ifs.close();
//    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to parse " + json));
//  }
//
//  ObjectFactory::VariantPrefab prefab;
//  prefab.m_name = document[Serializer::JsonNameKey].GetString();
//  // iterate through component objects in json array
//  for (rapidjson::SizeType i = 0; i < document[Serializer::JsonComponentsKey].GetArray().Size(); ++i)
//  {
//    const rapidjson::Value& component = document[Serializer::JsonComponentsKey].GetArray()[i];
//    for (rapidjson::Value::ConstMemberIterator iter{ component.MemberBegin() }; iter != component.MemberEnd(); ++iter)
//    {
//      // extract component name to determine the component type
//      std::string const compName{ iter->name.GetString() };
//      rttr::variant compVar{ rttr::type::get_by_name(compName).create({}) };
//      if (compVar.get_type().is_wrapper()) { compVar = compVar.extract_wrapped_value(); }
//      
//      rttr::type compType{ compVar.get_type() };
//      
//      prefab.m_components.emplace_back(compVar);
//    }
//  }
//
//  return prefab;
//}

void Deserializer::DeserializeBasedOnType(rttr::instance object, rapidjson::Value const& value)
{
  rttr::instance objInstance
  {
    object.get_type().get_raw_type().is_wrapper() ? object.get_wrapped_instance() : object
  };
  auto const properties{ objInstance.get_derived_type().get_properties() };
  std::cout << objInstance.get_type().get_name().to_string() << "\n";
  for (auto const& prop : properties)
  {
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
      DeserializeBasedOnType(ret, value);
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
        DeserializeSequentialContainer(view, value);
      }
      else if (prop.get_type().is_associative_container())
      {
        GE::Debug::ErrorLogger::GetInstance().LogMessage(
          prop.get_name().to_string() + ": Associative Containers are not yet supported");
      }
      break;
    }
    default:
    {
      rttr::variant var{ DeserializeBasicTypes(value) };
      if (var.convert(prop.get_type()))
      {
        prop.set_value(objInstance, var);
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

void Deserializer::DeserializeSequentialContainer(rttr::variant_sequential_view const& view, rapidjson::Value const& value)
{
  for (rapidjson::Value const& elem : value.GetArray())
  {
    if (!elem.IsArray() || !elem.IsObject())
    {
      std::cout << "not array or object!\n";
    }
      rttr::type const elemType{ view.get_value_type() };
      rttr::variant var{};
      // recursively deserialize the element if still a container
      if (elemType.is_sequential_container())
      {
        DeserializeSequentialContainer(var.create_sequential_view(), elem);
      }
      else {  // else deserialize normally
        DeserializeElement(var.get_type(), elem);
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
    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to read " + json));
  }

  rapidjson::Document document{};
  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (document.ParseStream(isw).HasParseError())
  {
    ifs.close();
    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg("Unable to parse " + json));
  }

  // check if root is object
  if (!document.IsObject())
  {
    ifs.close();
    throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg(json + ": root is not an object"));
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
        std::string str{ "Invalid component in " };
        str += json + ": " + component.GetString();
        throw Debug::Exception<std::ifstream>(Debug::LEVEL_CRITICAL, ErrMsg(str));
      }

      sig[static_cast<unsigned>(compType->second)] = true;
    }
    ret.emplace_back(system.name.GetString(), sig); // add entry to container
  }

  ifs.close();
  return ret;
}
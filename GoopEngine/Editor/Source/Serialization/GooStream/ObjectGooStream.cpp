/*!*********************************************************************
\file   ObjectGooStream.cpp
\author chengen.lau\@digipen.edu
\date   22-September-2023
\brief  GooStream class used for serializing / deserializing of a file
        of multiple entities (scene file), together with its individual
        components and their values. The class overrides both input
        and output functions.

        For the format of the json file, you can refer to
        Assets/Scenes/Scene.json
        For more details on usage, refer to GooStream.h
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ObjectGooStream.h"
#include <rapidjson/istreamwrapper.h>
#include <Serialization/Serializer.h>

using namespace GE::Serialization;

ObjectGooStream::ObjectGooStream(std::string const& json) : GooStream(true)
{
  m_elements = 0;
  Read(json);
}

bool ObjectGooStream::Read(std::string const& json)
{
  std::ifstream ifs{ json };
  if (!ifs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + json);
    #ifdef _DEBUG
    std::cout << "ObjectGooStream: Unable to read " + json << std::endl;
    #endif
    return m_status = false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (data.ParseStream(isw).HasParseError())
  {
    ifs.close();
    GE::Debug::ErrorLogger::GetInstance().LogError("JSON parse error: " + json);

    #ifdef _DEBUG
    std::cout << "ObjectGooStream: Unable to read " << json << std::endl;
    #endif
    return m_status = false;
  }

  // check if root of document is in array format 
  if (!data.IsArray())
  {
    ifs.close();
    std::ostringstream oss{};
    oss << json << ": root is not an Array";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return m_status = false;
  }

  // iterate through all objects and validate format
  for (rapidjson::SizeType i{}; i < data.Size(); ++i)
  {
    rapidjson::Value const& obj{ data[i] };

    if (!obj.IsObject())
    {
      ifs.close();
      std::ostringstream oss{};
      oss << json << ": Element " << std::to_string(i) << " is not an object";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return m_status = false;
    }
    if (!obj.HasMember(Serializer::JsonNameKey) || !obj[Serializer::JsonNameKey].IsString())
    {
      ifs.close();
      std::ostringstream oss{};
      oss << json << ": Element " << std::to_string(i) << " does not have a valid \"" << Serializer::JsonNameKey << "\" field";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return m_status = false;
    }
    
    if (!obj.HasMember(Serializer::JsonParentKey))
    {
      ifs.close();
      std::ostringstream oss{};
      oss << json << ": Element " << std::to_string(i) << " does not have a valid \"" << Serializer::JsonParentKey << "\" field";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return m_status = false;
    }

    if (!obj.HasMember(Serializer::JsonChildEntitiesKey) || !obj[Serializer::JsonChildEntitiesKey].IsArray())
    {
      ifs.close();
      std::ostringstream oss{};
      oss << json << ": Element " << std::to_string(i) << " does not have a valid \"" << Serializer::JsonChildEntitiesKey << "\" field";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return m_status = false;
    }

    if (!obj.HasMember(Serializer::JsonComponentsKey) || !obj[Serializer::JsonComponentsKey].IsArray())
    {
      ifs.close();
      std::ostringstream oss{};
      oss << json << ": Element " << std::to_string(i) << " does not have a valid \"" << Serializer::JsonComponentsKey << "\" field";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      return m_status = false;
    }

    rapidjson::Value const& componentArr{ obj[Serializer::JsonComponentsKey] };
    for (rapidjson::SizeType j{}; j < componentArr.Size(); ++j)
    {
      rapidjson::Value const& component{ componentArr[j] };
      if (!component.IsObject())
      {
        ifs.close();
        std::ostringstream oss{};
        oss << json << ": Component " << std::to_string(j) << " of element " << std::to_string(i) << " is not an object";
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
        #ifdef _DEBUG
        std::cout << oss.str() << std::endl;
        #endif
        return m_status = false;
      }
    }
  }

  ifs.close();
  m_elements = data.Size();
  return m_status = true;
}

bool ObjectGooStream::Read(container_type const& container)
{
  if (!m_status) {
    m_status = false;
    std::ostringstream oss{ "AssetGooStream corrupted before reading from " };
    oss << typeid(container).name();
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  // create an array of objects
  data.SetArray();
  for (container_type::value_type const& obj : container) // for each object
  {
    ObjectFactory::ObjectData const& objData{ obj.second };
    rapidjson::Value objVal{ rapidjson::kObjectType };  // create section for objects
    rapidjson::Value name{};
    name.SetString(Serializer::JsonNameKey, data.GetAllocator());
    objVal.AddMember(name, rapidjson::StringRef(obj.first.c_str()), data.GetAllocator()); // Add the name of the object

    rapidjson::Value componentBlock{ rapidjson::kArrayType };  // create array for components
    for (ObjectFactory::ComponentMap::value_type const& component : objData.m_components) // for each component in the object
    {
      rapidjson::Value componentObj{ rapidjson::kObjectType };  // create section for fields
      rapidjson::Document componentData;
      componentData.Parse(component.second.c_str());

      for (auto iter{ componentData.MemberBegin() }; iter != componentData.MemberEnd(); ++iter)
      {
        // Add each field to the component object
        componentObj.AddMember(iter->name, iter->value, data.GetAllocator());
      }

      // Add the component object to the array
      componentBlock.PushBack(componentObj, data.GetAllocator());
    }
    rapidjson::Value compName{};
    compName.SetString(Serializer::JsonComponentsKey, data.GetAllocator());
    objVal.AddMember(compName, componentBlock, data.GetAllocator());  // array label
    data.PushBack(objVal, data.GetAllocator());
  }

  return m_status = true;
}

bool ObjectGooStream::Unload(container_type& container)
{
  if (!m_status) {
    m_status = false;
    std::ostringstream oss{ "ObjectGooStream corrupted before unloading into " };
    oss << typeid(container).name();
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };
  
  // for each entity in entity array
  for (auto const& obj : data.GetArray())
  {
    ObjectFactory::ObjectData objData{
      (obj[Serializer::JsonParentKey].IsNull()) ? ECS::INVALID_ID : obj[Serializer::JsonParentKey].GetUint()
    };

    std::vector<ECS::Entity>& childEntities{ objData.m_childEntities };
    ObjectFactory::ComponentMap& compMap{ objData.m_components };

    rapidjson::Value const& childEntitiesArr = obj[Serializer::JsonChildEntitiesKey];
    for (rapidjson::Value const& child : childEntitiesArr.GetArray())
    {
      childEntities.emplace_back(child.GetUint());
    }

    // for each component in component array
    rapidjson::Value const& componentsArray = obj[Serializer::JsonComponentsKey];
    for (rapidjson::SizeType i{}; i < componentsArray.Size(); ++i)
    {
      // for each member in component object
      rapidjson::Value const& componentObject = componentsArray[i];
      for (rapidjson::Value::ConstMemberIterator iter{ componentObject.MemberBegin() }; iter != componentObject.MemberEnd(); ++iter)
      {
        ECS::COMPONENT_TYPES const name{ ECS::stringToComponents.at(iter->name.GetString()) };
        // set current component's bit in signature
        objData.m_componentSignature[static_cast<unsigned>(name)] = true;

        // Convert to json string and store in map
        rapidjson::StringBuffer buffer{};
        writer_type<rapidjson::StringBuffer> writer{ buffer };
        componentObject.Accept(writer);
        compMap[name] = buffer.GetString();
      }
    }
    container.emplace_back(obj[Serializer::JsonNameKey].GetString(), objData);
  }

  return m_status = true;
}

bool ObjectGooStream::Unload(std::string const& json, bool overwrite)
{
  if (!m_status) {
    m_status = false;
    std::ostringstream oss{ "ObjectGooStream corrupted before unloading into " };
    oss << json;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  std::ofstream ofs{ json, ((overwrite) ? std::ios::out : std::ios::app) };
  if (!ofs)
  {
    m_status = false;
    std::ostringstream oss{ "Unable to create output file " };
    oss << json;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return false;
  }

  rapidjson::OStreamWrapper osw{ ofs };
  writer_type<rapidjson::OStreamWrapper> writer(osw);
  data.Accept(writer);

  ofs.close();
  return m_status = true;
}
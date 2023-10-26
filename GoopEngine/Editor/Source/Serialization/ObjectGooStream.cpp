/*!*********************************************************************
\file   ObjectGooStream.cpp
\author chengen.lau\@digipen.edu
\date   22-September-2023
\brief  GooStream class used for serializing / deserializing of a file
        of multiple entities (scene file), together with its individual
        components and their values. The class overrides both input
        and output functions.

        For the format of the json file, you can refer to
        Assets/Data/Scene.json
        For more details on usage, refer to GooStream.h
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ObjectGooStream.h"
#include <rapidjson/istreamwrapper.h>

using namespace GE::Serialization;

// MUST CORRESPOND TO KEYS IN .json
const char ObjectGooStream::JsonNameKey[]       = "Name";
const char ObjectGooStream::JsonComponentsKey[] = "Components";

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

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
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
      #ifdef _DEBUG
      std::cout << oss.str() << std::endl;
      #endif
      return m_status = false;
    }
    if (!obj.HasMember(JsonNameKey) || !obj[JsonNameKey].IsString())
    {
      ifs.close();
      std::ostringstream oss{};
      oss << json << ": Element " << std::to_string(i) << " does not have a valid \"Name\" field";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      #ifdef _DEBUG
      std::cout << oss.str() << std::endl;
      #endif
      return m_status = false;
    }
    
    if (!obj.HasMember(JsonComponentsKey) || !obj[JsonComponentsKey].IsArray())
    {
      ifs.close();
      std::ostringstream oss{};
      oss << json << ": Element " << std::to_string(i) << " does not have a valid \"" << JsonComponentsKey << "\" field";
      GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      #ifdef _DEBUG
      std::cout << oss.str() << std::endl;
      #endif
      return m_status = false;
    }

    rapidjson::Value const& componentArr{ obj[JsonComponentsKey] };
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

  #ifdef _DEBUG
  std::cout << json << " successfully read" << "\n";
  #endif

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

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  // create an array of objects
  data.SetArray();
  for (container_type::value_type const& obj : container) // for each object
  {
    ObjectFactory::ObjectData const& objData{ obj.second };
    rapidjson::Value objVal{ rapidjson::kObjectType };  // create section for objects
    objVal.AddMember(JsonNameKey, rapidjson::StringRef(obj.first.c_str()), data.GetAllocator()); // Add the name of the object
    // serialize component signature as a number
    //objVal.AddMember("Signature", static_cast<unsigned>(objData.m_componentSignature.to_ulong()), data.GetAllocator());
    // create systems array

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

    objVal.AddMember(JsonComponentsKey, componentBlock, data.GetAllocator());  // array label
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

    #ifdef _DEBUG
    std::cout << oss.str() << std::endl;
    #endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };
  
  for (auto const& obj : data.GetArray())
  {
    ObjectFactory::ObjectData objData{};
    ObjectFactory::ComponentMap& compMap{ objData.m_components };
    
    const rapidjson::Value& componentsArray = obj[JsonComponentsKey];
    for (rapidjson::SizeType i{}; i < componentsArray.Size(); ++i)
    {
      const rapidjson::Value& componentObject = componentsArray[i];
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
    container.emplace(obj[JsonNameKey].GetString(), objData);
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

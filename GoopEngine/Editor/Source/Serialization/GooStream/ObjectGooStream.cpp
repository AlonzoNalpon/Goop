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
#include <Serialization/Deserializer.h>

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
  if (ifs.peek() == std::ifstream::traits_type::eof())
  {
    GE::Debug::ErrorLogger::GetInstance().LogMessage("Deserializing and read empty file. Ignoring checks");
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
  m_status = Serialization::Deserializer::ScanJsonFileForMembers(data, 5,
    Serializer::JsonNameKey, rapidjson::kStringType, Serializer::JsonChildEntitiesKey, rapidjson::kArrayType,
    Serializer::JsonIdKey, rapidjson::kNumberType, Serializer::JsonIdKey, rapidjson::kNumberType, Serializer::JsonParentKey,
    rapidjson::kNumberType, Serializer::JsonComponentsKey, rapidjson::kArrayType);

  ifs.close();
  m_elements = data.Size();
  return m_status;
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
      (obj[Serializer::JsonParentKey].IsNull()) ? ECS::INVALID_ID : obj[Serializer::JsonParentKey].GetUint(),
      obj[Serializer::JsonNameKey].GetString()
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
        auto const compType{ ECS::stringToComponents.find(iter->name.GetString()) };
        if (compType == ECS::stringToComponents.cend())
        {
          std::ostringstream oss{};
          oss << "Trying to deserialize invalid component: " << iter->name.GetString();
          GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
          #ifdef _DEBUG
          std::cout << oss.str() << "\n";
          #endif
          continue;
        }
        // set current component's bit in signature
        objData.m_componentSignature[static_cast<unsigned>(compType->second)] = true;

        // Convert to json string and store in map
        rapidjson::StringBuffer buffer{};
        writer_type<rapidjson::StringBuffer> writer{ buffer };
        componentObject.Accept(writer);
        compMap[compType->second] = buffer.GetString();
      }
    }
    container.emplace_back(obj[Serializer::JsonIdKey].GetUint(), std::move(objData));
  }

  return m_status = true;
}

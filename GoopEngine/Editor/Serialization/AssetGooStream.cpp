/*!*********************************************************************
\file   AssetGooStream.cpp
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include "AssetGooStream.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>
#ifdef _DEBUG
#include <iostream>
#endif

using namespace GE::Serialization;

AssetGooStream::AssetGooStream(std::string const& json) : GooStream(true)
{
  m_elements = 0;
  Read(json);
}

bool AssetGooStream::Read(std::string const& json)
{
  std::ifstream ifs{ json };
  if (!ifs)
  {
#ifdef _DEBUG
    std::cout << "Error: Unable to load " << json << "\n";
#endif
    return m_status = false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (data.ParseStream(isw).HasParseError())
  {
    ifs.close();

#ifdef _DEBUG
    std::cout << "JSON parse error: " << rapidjson::GetParseErrorFunc(data.GetParseError()) << "\n";
#endif
    return m_status = false;
  }

  // if data is not in the format of key-value pairs
  // i.e. "key": "value"
  if (!data.IsObject())
  {
    ifs.close();

#ifdef _DEBUG
    std::cerr << "JSON parse error: " << json << " is not in the right format " << "\n";
#endif
    return m_status = false;
  }

#ifdef SERIALIZE_TEST
  std::cout << json << " successfully read" << "\n";
#endif

  ifs.close();
  m_elements = data.MemberCount();
  return m_status = true;
}

bool AssetGooStream::Read(container_type const& container)
{
  if (!m_status) {
#ifdef _DEBUG
    std::cout << "AssetGooStream corrupted before unload\n";
#endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  for (container_type::value_type const& entry : container)
  {
    const char* key{ entry.first.c_str() };
    rapidjson::Value jsonKey{ key, data.GetAllocator() };
    rapidjson::Value value{ entry.second.c_str(), data.GetAllocator() };

    // Append to m_data or overwrite if key already exists
    if (!data.HasMember(key))
    {
      data.AddMember(jsonKey, value, data.GetAllocator());
    }
    else
    {
      data[key] = value;
    }
  }

  m_elements = data.Size();
  return m_status = true;
}

bool AssetGooStream::Unload(container_type& container)
{
  if (!m_status) {
#ifdef _DEBUG
    std::cout << "AssetGooStream corrupted before unload\n";
#endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  for (rapidjson::Value::ConstMemberIterator it{ data.MemberBegin() };
    it != data.MemberEnd(); ++it)
  {
    container[it->name.GetString()] = it->value.GetString();
  }

  return m_status = true;
}

bool AssetGooStream::Unload(std::string const& json, bool overwrite)
{
  if (!m_status) {
#ifdef _DEBUG
    std::cout << "AssetGooStream corrupted before unload\n";
#endif
    return false;
  }
  rapidjson::Document& data{ std::get<rapidjson::Document>(m_data) };

  std::ofstream ofs{ json, ((overwrite) ? std::ios::out : std::ios::app) };
  if (!ofs)
  {
#ifdef _DEBUG
    std::cout << "Unable to create output file " << json << "\n";
#endif
    return m_status = false;
  }

  rapidjson::OStreamWrapper osw{ ofs };
  writer_type<rapidjson::OStreamWrapper> writer(osw);
  data.Accept(writer);

  ofs.close();
  return m_status = true;
}
/*!*********************************************************************
\file   SpriteGooStream.cpp
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include "SpriteGooStream.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>
#ifdef _DEBUG
#include <iostream>
#endif

using namespace GE::Serialization;

SpriteGooStream::SpriteGooStream(std::string const& json) : GooStream()
{
  Read(json);
}

bool SpriteGooStream::Read(std::string const& json)
{
  std::ifstream ifs{ json };
  if (!ifs)
  {
#ifdef _DEBUG
    std::cout << "Error: Unable to load " << json << "\n";
#endif
    return m_status = false;
  }

  // parse into document object
  rapidjson::IStreamWrapper isw{ ifs };
  if (m_data.ParseStream(isw).HasParseError())
  {
    ifs.close();

#ifdef _DEBUG
    std::cout << "JSON parse error: " << rapidjson::GetParseErrorFunc(m_data.GetParseError()) << "\n";
#endif
    return m_status = false;
  }

  // if data is in the format of key-value pairs
  // i.e. "key": "value"
  if (!m_data.IsObject())
  {
    ifs.close();

#ifdef _DEBUG
    std::cerr << "JSON parse error: " << json << " is not in the right format " << "\n";
#endif
    return m_status = false;
  }

#ifdef SERIALIZE_TEST
  std::cout << ASSETS_DIR + json << " successfully read" << "\n";
#endif

  ifs.close();
  return m_status = true;
}

bool SpriteGooStream::Read(container_type const& container)
{
  if (!m_status) {
#ifdef _DEBUG
    std::cout << "SpriteGooStream corrupted before unload\n";
#endif
    return false;
  }

  for (container_type::value_type const& entry : container)
  {
    const char* key{ entry.first.c_str() };
    rapidjson::Value jsonKey{ key, m_data.GetAllocator() };
    rapidjson::Value value{ entry.second.c_str(), m_data.GetAllocator() };

    // Append to m_data or overwrite if key already exists
    if (!m_data.HasMember(key))
    {
      m_data.AddMember(jsonKey, value, m_data.GetAllocator());
    }
    else
    {
      m_data[key] = value;
    }
  }

  return m_status = true;
}

bool SpriteGooStream::Unload(container_type& container)
{
  if (!m_status) {
#ifdef _DEBUG
    std::cout << "SpriteGooStream corrupted before unload\n";
#endif
    return false;
  }

  for (rapidjson::Value::ConstMemberIterator it{ m_data.MemberBegin() };
    it != m_data.MemberEnd(); ++it)
  {
    container[it->name.GetString()] = it->value.GetString();
  }

  return m_status = true;
}

bool SpriteGooStream::Unload(std::string const& json, bool overwrite)
{
  if (!m_status) {
#ifdef _DEBUG
    std::cout << "SpriteGooStream corrupted before unload\n";
#endif
    return false;
  }

  std::ofstream ofs{ json, ((overwrite) ? std::ios::out : std::ios::app) };
  if (!ofs)
  {
#ifdef _DEBUG
    std::cout << "Unable to create output file " << json << "\n";
#endif
    return m_status = false;
  }

  rapidjson::OStreamWrapper osw{ ofs };
  writer_type writer(osw);
  m_data.Accept(writer);

  ofs.close();
  return m_status = true;
}

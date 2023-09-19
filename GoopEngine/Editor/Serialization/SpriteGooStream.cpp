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

bool SpriteGooStream::Read(std::string const& file)
{
  std::ifstream ifs{ file };
  if (!ifs)
  {
    #ifdef _DEBUG
    std::cout << "Error: Unable to load " << file << "\n";
    #endif
    return m_status = false;
  }

  std::string line{};
  std::getline(ifs, line);  // Skip first line
  while (std::getline(ifs, line))
  {
    // skip line if comment
    if (line.empty() || line.front() == CommentSymbol) { continue; }

    m_data << line << "\n";
    ++m_elements;
  }
  

  #ifdef SERIALIZE_TEST
  std::cout << json << " successfully read" << "\n";
  #endif

  ifs.close();
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
  std::string const ossData{ m_data.str() };
  std::istringstream iss{ ossData };

  std::string line;
  while (std::getline(iss, line))
  {
    SpriteData sprite{};
    std::istringstream lineStream(line);
    lineStream >> sprite.m_id >> sprite.m_filePath >> sprite.m_slices
      >> sprite.m_stacks >> sprite.m_frames;
    container.emplace_back(sprite);
  }

  return m_status = true;
}

//--------------------------------------------//
// FOR FUTURE IF NEEDED: JSON format reading  //
//--------------------------------------------//
// JSON key values
//static constexpr const char* JsonName = "name";
//static constexpr const char* JsonFilePath = "filepath";
//static constexpr const char* JsonSlices = "slices";
//static constexpr const char* JsonStacks = "stacks";
//static constexpr const char* JsonFrames = "frames";
//bool SpriteGooStream::Read(std::string const& json)
//{
//  std::ifstream ifs{ json };
//  if (!ifs)
//  {
//#ifdef _DEBUG
//    std::cout << "Error: Unable to load " << json << "\n";
//#endif
//    return m_status = false;
//  }
//
//  // parse into document object
//  rapidjson::IStreamWrapper isw{ ifs };
//  if (m_data.ParseStream(isw).HasParseError())
//  {
//    ifs.close();
//
//#ifdef _DEBUG
//    std::cout << "JSON parse error: " << rapidjson::GetParseErrorFunc(m_data.GetParseError()) << "\n";
//#endif
//    return m_status = false;
//  }
//
//  // if data is not in the format of arrays
//  // i.e.{ {"": ""}, {"": ""}, {"": ""} }
//  if (!m_data.IsArray())
//  {
//    ifs.close();
//
//#ifdef _DEBUG
//    std::cerr << "JSON parse error: " << json << " is not in the right format " << "\n";
//#endif
//    return m_status = false;
//  }
//
//#ifdef SERIALIZE_TEST
//  std::cout << json << " successfully read" << "\n";
//#endif
//
//  ifs.close();
//  return m_status = true;
//}
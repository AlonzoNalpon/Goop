/*!*********************************************************************
\file   SpriteGooStream.h
\author chengen.lau\@digipen.edu
\date   18-September-2023
\brief  GooStream class used for deserializing of a text file containing
        sprite animation data. The data of each sprite consists of its
        slices, stacks and frames of the sprite sheet as well as the
        animation speed. The class overrides only input functions.

        For the format of the txt file, you can refer to
        Assets/Data/Sprites.txt
        For more details on usage, refer to GooStream.h

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "SpriteGooStream.h"
#include <fstream>
#include <rapidjson/istreamwrapper.h>

using namespace GE::Serialization;

SpriteGooStream::SpriteGooStream(std::string const& file) : GooStream(false)
{
  m_elements = 0;
  Read(file);
}

bool SpriteGooStream::Read(std::string const& file)
{
  std::ifstream ifs{ file };
  if (!ifs)
  {
    GE::Debug::ErrorLogger::GetInstance().LogError("Unable to read " + file);
    #ifdef _DEBUG
    std::cout << "SpriteGooStream: Unable to read " + file << std::endl;
    #endif
    return m_status = false;
  }
  std::ostringstream& data{ std::get<std::ostringstream>(m_data) };

  std::string line{};
  std::getline(ifs, line);  // Skip first line
  while (std::getline(ifs, line))
  {
    // skip line if comment
    if (line.empty() || line.front() == CommentSymbol) { continue; }

    data << line << "\n";
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
    std::ostringstream oss{ "SpriteGooStream corrupted before unloading into " };
    oss << typeid(container).name();
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());

    #ifdef _DEBUG
    std::cout <<  oss.str() << std::endl;
    #endif
    return false;
  }
  std::ostringstream& data{ std::get<std::ostringstream>(m_data) };
  std::string const ossData{ data.str() };
  std::istringstream iss{ ossData };

  std::string line;
  while (std::getline(iss, line))
  {
    SpriteData sprite{};
    std::istringstream lineStream(line);
    lineStream >> sprite.m_id >> sprite.m_filePath >> sprite.m_slices
      >> sprite.m_stacks >> sprite.m_frames >> sprite.m_speed;
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
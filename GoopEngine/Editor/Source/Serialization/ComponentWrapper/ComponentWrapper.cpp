/*!*********************************************************************
\file   ComponentWrapper.cpp
\author chengen.lau\@digipen.edu
\date   15-October-2023
\brief  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include "ComponentWrapper.h"
#include <AssetManager/AssetManager.h>

using namespace GE;
using namespace Serialization;

ComponentWrapper::ComponentWrapper(std::string const& componentData) : m_data{}
{
  if (m_data.Parse(componentData.c_str()).HasParseError())
  {
    throw Debug::Exception<ComponentWrapper>(Debug::LEVEL_ERROR, ErrMsg("Unable to parse component data"));
  }
  m_ptr = &m_data.MemberBegin()->value;
}

ComponentWrapper::ComponentWrapper(rapidjson::Value const& componentData) : m_data{}
{
  m_data.CopyFrom(componentData, m_data.GetAllocator());
  m_ptr = &m_data.MemberBegin()->value;
}

// base subscript operator definition to fallback to
template <typename T>
T ComponentWrapper::Get(const char* key) const
{
  std::ostringstream oss{};
  oss << "Trying to deserialize " << key << " into unsupported type " << typeid(T).name();
  GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  return T();
}

template <>
bool ComponentWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetBool();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to bool";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return false;
  }
}

template <>
float ComponentWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetFloat();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to float";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0.0;
  }
}

template <>
double ComponentWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetDouble();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to double";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0.0;
  }
}

template <>
int ComponentWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetInt();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to int";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0;
  }
}

template <>
unsigned ComponentWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetUint();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to unsigned";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0;
  }
}

template <>
size_t ComponentWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetUint64();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to size_t";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return 0;
  }
}

template <>
const char* ComponentWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to const char*";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return nullptr;
  }
}

template <>
std::string ComponentWrapper::Get(const char* key) const
{
  try
  {
    return (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to std::string";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return std::string();
  }
}

template <>
Math::Vec2 ComponentWrapper::Get(const char* key) const
{
  try
  {
    Math::Vec2 vec{};
    return vec << (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to Math::Vec2";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return Math::Vec2();
  }
}

template <>
Math::dVec2 ComponentWrapper::Get(const char* key) const
{
  try
  {
    Math::dVec2 vec{};
    return vec << (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to Math::dVec2";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return Math::dVec2();
  }
}

template<> Math::dVec3 ComponentWrapper::Get(const char* key) const
{
  try
  {
    Math::dVec3 vec{};
    return vec << (*m_ptr)[key].GetString();
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to Math::dVec3";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return Math::dVec3();
  }
}

template <>
std::queue<Math::dVec2> ComponentWrapper::Get(const char* key) const
{
  try
  {
    std::queue<Math::dVec2> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec2 vec{};
      vec << elem.GetString();
      ret.emplace(std::move(vec));
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to std::queue<Math::dVec2>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template <>
std::deque<Math::dVec2> ComponentWrapper::Get(const char* key) const
{
  try
  {
    std::deque<Math::dVec2> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec2 vec{};
      vec << elem.GetString();
      ret.emplace_back(std::move(vec));
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to std::deque<Math::dVec2>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template <>
std::queue<Math::dVec3> ComponentWrapper::Get(const char* key) const
{
  try
  {
    std::queue<Math::dVec3> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec3 vec{};
      vec << elem.GetString();
      ret.emplace(std::move(vec));
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to std::queue<Math::dVec3>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template <>
std::deque<Math::dVec3> ComponentWrapper::Get(const char* key) const
{
  try
  {
    std::deque<Math::dVec3> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec3 vec{};
      vec << elem.GetString();
      ret.emplace_back(std::move(vec));
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to std::deque<Math::dVec3>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template <>
std::deque<GE::Component::Tween::Action> ComponentWrapper::Get(const char* key) const
{
  try
  {
    std::deque<GE::Component::Tween::Action> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec3 tgt{};
      tgt << elem["target"].GetString();

      ret.emplace_back(tgt, elem["duration"].GetDouble());
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to std::deque<Math::dVec3>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template<>
std::vector<std::string>
ComponentWrapper::Get(const char* key) const
{
  try
  {
    std::vector<std::string> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      ret.emplace_back(elem.GetString());
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to std::vector<std::pair<std::string, std::string>>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template<>
Component::DragForce ComponentWrapper::Get(const char* key) const
{
  try
  {
    rapidjson::Value const& list{ (*m_ptr)[key] };
    Component::DragForce ret{ list["magnitude"].GetDouble(),  list["isActive"].GetBool() };

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to Component::DragForce";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

template<> std::vector<Component::LinearForce> ComponentWrapper::Get(const char* key) const
{
  try
  {
    std::vector<Component::LinearForce> ret{};
    rapidjson::Value const& list{ (*m_ptr)[key] };
    for (auto const& elem : list.GetArray())
    {
      Math::dVec3 vec;
      vec << elem["magnitude"].GetString();
      ret.emplace_back(
        vec, elem["lifetime"].GetDouble(), elem["isActive"].GetBool(), elem["age"].GetDouble()
      );
    }

    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to std::vector<Component::LinearForce>";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}


template<> Graphics::Colorf ComponentWrapper::Get(const char* key)const
{
  try
  {
    rapidjson::Value const& list{ (*m_ptr)[key] };
    Graphics::Colorf ret{ list["r"].GetFloat(), list["g"].GetFloat(), list["b"].GetFloat(), list["a"].GetFloat() };
    return ret;
  }
  catch (...)
  {
    std::ostringstream oss{};
    oss << "ComponentWrapper: Unable to convert value of key: " << key
      << " to Graphics::Colorf";
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return {};
  }
}

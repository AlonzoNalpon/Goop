
/*!*********************************************************************
\file   AssetManager.tpp
\author loh.j\@digipen.edu
\date   28 September 2023
\brief
  Contains the template functions of GetConfigData.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

/*!*********************************************************************
\brief
  Gets the config data of the specific key. Type of return value must
  be specified as template argument.
\param
  const std::string& key (Key of the config data in the Config.json)
\return
  Data of the specific key converted to type T
************************************************************************/
template <typename T>
T GetConfigData(const std::string& key) const
{
  if (m_configData.find(key) == m_configData.end())
  {
    std::ostringstream oss{};
    oss << "Unable to find \"" << key << "\" in config data";
    throw GE::Debug::Exception<AssetManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg(oss.str()));
  }

  T result;
  std::istringstream iss(m_configData.at(key));
  iss >> result;
  if (iss.bad())
  {
    std::ostringstream oss{};
    oss << "Unabled to convert config data of key \"" << key << " \" to " << typeid(T).name();
    throw GE::Debug::Exception<AssetManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg(oss.str()));
  }

  return result;
}

template <>
std::string GetConfigData(const std::string& key) const
{
  if (m_configData.find(key) == m_configData.end())
  {
    std::ostringstream oss{};
    oss << "Unable to find \"" << key << "\" in config data";
    throw GE::Debug::Exception<AssetManager>(GE::Debug::LEVEL_CRITICAL, ErrMsg(oss.str()));
  }

  return m_configData.at(key);
}
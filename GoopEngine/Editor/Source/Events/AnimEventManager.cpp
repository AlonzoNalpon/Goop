#include <pch.h>
#include <Events/AnimEventManager.h>
#include <AssetManager/AssetManager.h>
#include <Serialization/Serializer.h>
#include <Serialization/Deserializer.h>
#include <DebugTools/Exception/Exception.h>
namespace GE::Events
{
  AnimEventManager::AnimEventManager()
  {
    LoadDatabase();
  }

  bool AnimEventManager::CreateAnimEvents(std::string const& name,
    AnimEvents::AnimEventsCont const& eventList, bool overwrite)
  {
    auto animIt = m_animTable.find(name);
    // Check if it exists and return false is not to be overwritten
    if (!overwrite && animIt != m_animTable.end())
      return false;

    m_animTable[name] = eventList;  // add to the animation table
    SaveDatabase();
    return true;
  }

  AnimEventManager::AnimEventsTable const& AnimEventManager::GetAnimEventsTable() const
  {
    return m_animTable;
  }

  AnimEvents::AnimEventsCont const& AnimEventManager::GetAnimEvent(std::string const& name)const
  {
    auto animIt = m_animTable.find(name);
    if (animIt == m_animTable.end())
      throw GE::Debug::Exception<AnimEventManager>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("Anim event holder of name does not exist: " + name));

    return animIt->second; // return the object
  }

  void AnimEventManager::SaveDatabase() const
  {
    std::string const dataFilePath = GE::Assets::AssetManager::GetInstance().
      GetConfigData<std::string>("AnimEvents");
    Serialization::Serializer::SerializeAny(dataFilePath, m_animTable);
  }
  void AnimEventManager::LoadDatabase()
  {
    std::string const dataFilePath = GE::Assets::AssetManager::GetInstance().
      GetConfigData<std::string>("AnimEvents");
    m_animTable = Serialization::Deserializer::DeserializeAnimEventsTable(dataFilePath);
  }
}
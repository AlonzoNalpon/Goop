#include <pch.h>
#include <Fmod/FmodSystem.h>
#include <fmod_errors.h>
#include <AssetManager/AssetManager.h>
#include <filesystem>

using namespace GE::fMOD;

FmodSystem::~FmodSystem()
{
  m_sounds.clear();
  m_channels.clear();
  std::for_each(m_sounds.begin(), m_sounds.end(), [](auto& s) { s.second->release(); });
  // if fmod not initialized dont close 
  if (m_fModSystem != nullptr)
  {
    ErrorCheck(m_fModSystem->close());
  }
}

void FmodSystem::Init()
{
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::WindowGainFocusEvent>(this);
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::WindowLoseFocusEvent>(this);

  ErrorCheck(FMOD::System_Create(&m_fModSystem)); // Create the FMOD Core system
  int maxSounds = GE::Assets::AssetManager::GetInstance().GetConfigData<int>("MaxPlayingSounds");
  ErrorCheck(m_fModSystem->init(maxSounds, FMOD_INIT_STREAM_FROM_UPDATE, NULL)); // Initialize the FMOD Core system
  m_fModSystem->getMasterChannelGroup(&m_masterGroup);

  for (int i{}; i < TOTAL_CHANNELS; ++i)
  {
    ChannelType currChannel = static_cast<ChannelType>(i);
    FMOD::ChannelGroup*& channelGroup{m_channelGroups[currChannel]};
    m_fModSystem->createChannelGroup(m_channelToString.at(currChannel).c_str(), &channelGroup);
    // This should be set from config file
    //channelGroup->setVolume(1.0f);
    m_volumes[currChannel] = 1.0f;

    m_masterGroup->addGroup(m_channelGroups[currChannel]);
  }
}

void FmodSystem::Update()
{
  ErrorCheck(m_fModSystem->update());
}

bool FmodSystem::LoadSound(std::string audio, bool looped)
{
  // Check if the sound is already loaded
  auto soundFound = m_sounds.find(audio);
  if (soundFound != m_sounds.end())
  {
    return true;
  }

  FMOD_MODE mode = FMOD_DEFAULT | FMOD_CREATESTREAM | (looped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

  FMOD::Sound* sound = nullptr;
  static GE::Assets::AssetManager& am{ GE::Assets::AssetManager::GetInstance() };

  // Use FMOD Core API to create the sound
  FMOD_RESULT result = m_fModSystem->createSound(am.GetSound(audio).c_str(), mode, nullptr, &sound);
  if (result == FMOD_OK)
  {
    // sound loaded, can stop
    m_sounds[audio] = sound;
    return true;
  }
  else
  {
    std::ostringstream oss{};
    oss << "Failed to create looped sound: " << result;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
    return false;
  }
}

void FmodSystem::UnLoadSounds()
{
  std::vector<std::string> remove;
  for (auto& [key, val] : m_channels)
  {
    bool isPlaying;    
    
    ErrorCheck(val->isPlaying(&isPlaying));
    if (!isPlaying)
    {
      remove.push_back(key);
    }
  }

  for (auto& key : remove)
  {
    m_channels.erase(key);
  }
}

void FmodSystem::PlaySound(std::string audio, float volume, ChannelType channel, bool looped)
{
  SoundMap::iterator soundFound = m_sounds.find(audio);

  if (soundFound == m_sounds.end()) //if sound not found, load sound
  {
    LoadSound(audio, looped);
    soundFound = m_sounds.find(audio);
    if (soundFound == m_sounds.end()) //if sound still not found, return 
    {
      return;
    }
  }
  
  FMOD::Channel*& soundChannel{m_channels[audio]};

  if (soundChannel == nullptr)
  {
    ErrorCheck(m_fModSystem->playSound(soundFound->second, m_channelGroups[channel], false, &soundChannel));
    soundChannel->setVolume(volume);
  }
  else
  {
    bool isPaused{};
    try
    {
      ErrorCheck(soundChannel->getPaused(&isPaused));
      // if sound paused, unpause
      if (isPaused)
      {
        ErrorCheck(soundChannel->setPaused(false));
      }
      else
      {
        ErrorCheck(m_fModSystem->playSound(soundFound->second, m_channelGroups[channel], false, &soundChannel));
        soundChannel->setVolume(volume);
      }
    }
    catch (GE::Debug::IExceptionBase&)
    {
      // Catch the thrown, if there is a throw at pause it means the channel is not playing anymore
      // not an actual thrown error
      ErrorCheck(m_fModSystem->playSound(soundFound->second, m_channelGroups[channel], false, &soundChannel));
      soundChannel->setVolume(volume);
    }
  }
}

void GE::fMOD::FmodSystem::Pause(std::string audio)
{
  if (m_channels.find(audio) != m_channels.end())
  {
    try
    {
      ErrorCheck(m_channels[audio]->setPaused(true));
    }
    catch (GE::Debug::IExceptionBase&)
    {
      // Catch the throw, if there is a throw at pause it means the channel is not playing anymore
      // not an actual bad error
      // Do nothing
    }
  }
}

float GE::fMOD::FmodSystem::GetVolume(std::string audio)
{
  float vol{};
  if (m_channels.find(audio) != m_channels.end())
  {
    try
    {
      ErrorCheck(m_channels[audio]->getVolume(&vol));
    }
    catch (GE::Debug::IExceptionBase&)
    {
      // Catch the throw, if there is a throw at pause it means the channel is not playing anymore
      // not an actual bad error
      // Do nothing      
    }
  }

  return vol;
}

void FmodSystem::StopSound(std::string audio)
{  
  try
  {
    ErrorCheck(m_channels[audio]->stop());
  }
  catch (...)
  {
    // do nothing
  }
  m_channels.erase(audio);
}

void FmodSystem::StopAllSound()
{
  ErrorCheck(m_masterGroup->stop());
  m_channels.clear();
}

void FmodSystem::StopChannel(ChannelType channel)
{
  int numchannel;
  m_channelGroups[channel]->getNumChannels(&numchannel);
  // Remove all sounds in given channel
  std::vector<std::string> remove;
  for (int i{}; i < numchannel; ++i)
  {
    FMOD::Channel* currentChannel;
    m_channelGroups[channel]->getChannel(i, &currentChannel);

    for (auto& [key, val] : m_channels)
    {
      if (val == currentChannel)
      {
        remove.push_back(key);
      }
    }
  }

  for (auto& key : remove)
  {
    m_channels.erase(key);
  }
  ErrorCheck(m_channelGroups[channel]->stop());
}

void FmodSystem::SetChannelVolume(ChannelType channel, float volume)
{
  ErrorCheck(m_channelGroups[channel]->setVolume(volume));
  m_volumes[channel] = volume;
}

float FmodSystem::GetChannelVolume(ChannelType channel) const
{
  return m_volumes.at(channel);
}

void GE::fMOD::FmodSystem::SetVolume(std::string audio, float volume)
{
  m_channels[audio]->setVolume(volume);
}

void FmodSystem::SetMasterVolume(float volume)
{
  m_masterGroup->setVolume(volume);
}

float GE::fMOD::FmodSystem::GetMasterVolume() const
{
  float vol;
  m_masterGroup->getVolume(&vol);
  return vol;
}

float FmodSystem::dbToVolume(float dB) const
{
  return static_cast<float>(pow(10.0, 0.05 * dB));
}

float FmodSystem::VolumeTodb(float volume) const
{
  return static_cast<float>(20.0 * log10(volume));
}

void GE::fMOD::FmodSystem::HandleEvent(GE::Events::Event* event)
{
  switch (event->GetCategory())
  {
  case GE::Events::EVENT_TYPE::WINDOW_LOSE_FOCUS:
    m_masterGroup->setPaused(true);
    break;
  case GE::Events::EVENT_TYPE::WINDOW_GAIN_FOCUS:
    m_masterGroup->setPaused(false);
    break;
  default:
    break;
  }
}

void FmodSystem::ErrorCheck(FMOD_RESULT result)
{
  static GE::Debug::ErrorLogger& logger = GE::Debug::ErrorLogger::GetInstance();
  if (result != FMOD_OK)
  {
    std::string error = FMOD_ErrorString(result);
    throw Debug::Exception<FmodSystem>(Debug::LEVEL_ERROR, ErrMsg((error).c_str()));
  }
}
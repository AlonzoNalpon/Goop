#include <pch.h>
#include <Fmod/FmodSystem.h>
#include <AssetManager/AssetManager.h>
#include <filesystem>

using namespace GE::fMOD;

FmodSystem::~FmodSystem()
{
  m_sounds.clear();
  m_channels.clear();
  std::for_each(m_sounds.begin(), m_sounds.end(), [](auto& s) { s.second->release(); });
  ErrorCheck(m_fModSystem->close());
}

void FmodSystem::Init()
{
  ErrorCheck(FMOD::System_Create(&m_fModSystem)); // Create the FMOD Core system
  int maxSounds = GE::Assets::AssetManager::GetInstance().GetConfigData<int>("MaxPlayingSounds");
  ErrorCheck(m_fModSystem->init(maxSounds, FMOD_INIT_NORMAL, NULL)); // Initialize the FMOD Core system
  m_fModSystem->getMasterChannelGroup(&m_masterGroup);
}

void FmodSystem::Update()
{
  ErrorCheck(m_fModSystem->update());
  ClearChannels();
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

  // Look through asset directory to find the file
  static std::string rootDir = GE::Assets::AssetManager::GetInstance().GetConfigData<std::string>("Assets Dir");
  for (const auto& path : std::filesystem::recursive_directory_iterator(rootDir))
  {
    if (path.path().filename() == audio)
    {
      // Use FMOD Core API to create the sound
      FMOD_RESULT result = m_fModSystem->createSound(path.path().string().c_str(), mode, nullptr, &sound);
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
  }

  std::ostringstream oss{};
  oss << "Sound file" << audio << "does not exist";
  GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  return false;
}

void FmodSystem::ClearChannels()
{
  std::vector<std::string> remove;
  for (auto& [key, val] : m_channels)
  {
    bool isPlaying;
    val->isPlaying(&isPlaying);
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

void FmodSystem::PlaySound(std::string audio, ChannelType channel, bool looped)
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
  
  FMOD::ChannelGroup* fmodChannelGroup = m_channelGroups[channel];
  if (fmodChannelGroup == nullptr)
  {
    m_fModSystem->createChannelGroup(m_channelToString.at(channel).c_str(), &fmodChannelGroup);
  }

  FMOD::Channel*& soundChannel{m_channels[audio]};
  ErrorCheck(m_fModSystem->playSound(soundFound->second, fmodChannelGroup, true, &soundChannel));
  if (soundChannel)
  {
    ErrorCheck(soundChannel->setPaused(false));
  }
}

void FmodSystem::StopSound(std::string audio)
{  
  m_channels[audio]->stop();
}

void FmodSystem::StopAllSound()
{
  m_masterGroup->stop();
}

void FmodSystem::StopChannel(ChannelType channel)
{
  m_channelGroups[channel]->stop();
}

void FmodSystem::SetChannelVolume(ChannelType channel, double volumedB)
{
  ErrorCheck(m_channelGroups[channel]->setVolume(static_cast<float>(dbToVolume(volumedB))));
}

double FmodSystem::dbToVolume(double dB) const
{
  return pow(10.0, 0.05 * dB);
}

double FmodSystem::VolumeTodb(double volume) const
{
  return 20.0 * log10(volume);
}

void FmodSystem::ErrorCheck(FMOD_RESULT result)
{
  if (result != FMOD_OK)
  {
    std::string error = "FMOD ERROR " + result;
    throw Debug::Exception<FmodSystem>(Debug::LEVEL_ERROR, ErrMsg((error).c_str()));
  }
}
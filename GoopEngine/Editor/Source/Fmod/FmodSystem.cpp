#include <pch.h>
#include <Fmod/FmodSystem.h>
#include <AssetManager/AssetManager.h>
#include <filesystem>

using namespace GE::fMOD;

FmodSystem::FmodSystem()
{
  ErrorCheck(FMOD::System_Create(&m_fModSystem)); // Create the FMOD Core system
  ErrorCheck(m_fModSystem->init(TOTAL_CHANNELS, FMOD_INIT_NORMAL, NULL)); // Initialize the FMOD Core system
}

FmodSystem::~FmodSystem()
{
  m_sounds.clear();
  m_channels.clear();
  UnLoadSound();
  ErrorCheck(m_fModSystem->close());
}

void FmodSystem::Update()
{
  ErrorCheck(m_fModSystem->update());
  UnLoadSound();
}

void FmodSystem::LoadSound(std::string audio, bool looped)
{
  // Check if the sound is already loaded
  auto soundFound = m_sounds.find(audio);
  if (soundFound != m_sounds.end())
  {
    return;
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
        break;
      }
      else
      {
        std::ostringstream oss{};
        oss << "Failed to create looped sound: " << result;
        GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
      }
    }
  }
}

void FmodSystem::UnLoadSound()
{
  for (std::pair sound : m_playlist)
  {
    if (!IsPlaying(sound))
    {
      //erases sound from sound map
      auto soundFoundMap = m_sounds.find(sound.first);
      if (soundFoundMap == m_sounds.end())
      {
        return;
      }
      ErrorCheck(soundFoundMap->second->release());
      m_sounds.erase(soundFoundMap);

      //erases sound from playlist
      auto soundFoundPlaylist = m_playlist.find(sound.first);
      if (soundFoundPlaylist == m_playlist.end())
      {
        return;
      }
      m_playlist.erase(soundFoundPlaylist);
    }
  }
}

void FmodSystem::PlaySound(std::string audio, Channel channel, bool looped)
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
  
  FMOD::Channel* fmodChannel = m_channels[channel];
  m_playlist[audio] = channel;

  ErrorCheck(m_fModSystem->playSound(soundFound->second, nullptr, true, &fmodChannel));
  if (fmodChannel)
  {
    ErrorCheck(fmodChannel->setPaused(false));
    m_channels[channel] = fmodChannel;
  }
}

void FmodSystem::StopSound(std::string audio)
{  
  m_playlist.erase(audio);
}

void FmodSystem::StopAllSound()
{
  for (std::pair itr : m_channels)
  {
    StopChannel(itr.first);
  }
}

void FmodSystem::StopChannel(Channel channel)
{
  m_channels[channel]->stop();
}

void FmodSystem::SetChannelVolume(Channel channel, double volumedB)
{
  ErrorCheck(m_channels[channel]->setVolume(static_cast<float>(dbToVolume(volumedB))));
}

bool FmodSystem::IsPlaying(std::pair<std::string, Channel> audio)
{
  FMOD::Channel* channel = m_channels[audio.second];

  if (channel)
  {
    FMOD::Sound* currentSound = m_sounds.find(audio.first)->second;
    FMOD_RESULT result = channel->getCurrentSound(&currentSound);

    if (result == FMOD_OK && currentSound)
    {
      return true;
    }
  }
  return false;
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
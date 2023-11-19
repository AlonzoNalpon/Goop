#include <pch.h>
#include <Systems/Audio/FmodSystem.h>
#include <Component/Audio.h>
#include <iostream>

using namespace GE::fMOD;
using namespace GE::Component;

FmodSystem::FmodSystem()
{
  ErrorCheck(FMOD::System_Create(&m_fModSystem)); // Create the FMOD Core system
  ErrorCheck(m_fModSystem->init(m_numOfChannels, FMOD_INIT_NORMAL, NULL)); // Initialize the FMOD Core system
}

FmodSystem::~FmodSystem()
{
  m_sounds.clear();
  m_channels.clear();
  ErrorCheck(m_fModSystem->close());
}

void FmodSystem::Update()
{
  ErrorCheck(m_fModSystem->update());
  UnLoadSound();
}

void FmodSystem::LoadSound(std::string audio)
{
  // Check if the sound is already loaded
  auto soundFound = m_sounds.find(audio);
  if (soundFound != m_sounds.end())
  {
    return;
  }

  FMOD_MODE mode = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_OFF;

  FMOD::Sound* sound = nullptr;

  // Use FMOD Core API to create the sound
  FMOD_RESULT result = m_fModSystem->createSound(audio.c_str(), mode, nullptr, &sound);
  if (result == FMOD_OK)
  {
    m_sounds[audio] = sound;
  }
  else
  {
    std::ostringstream oss{};
    oss << "Failed to create normal sound: " << result;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }
}

void FmodSystem::LoadLoopedSound(std::string audio)
{
  // Check if the sound is already loaded
  auto soundFound = m_sounds.find(audio);
  if (soundFound != m_sounds.end())
  {
    return;
  }

  FMOD_MODE mode = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;

  FMOD::Sound* sound = nullptr;

  // Use FMOD Core API to create the sound
  FMOD_RESULT result = m_fModSystem->createSound(audio.c_str(), mode, nullptr, &sound);
  if (result == FMOD_OK)
  {
    m_sounds[audio] = sound;
  }
  else
  {
    std::ostringstream oss{};
    oss << "Failed to create looped sound: " << result;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
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

void FmodSystem::PlaySound(std::string audio, bool isSFX)
{
  SoundMap::iterator soundFound = m_sounds.find(audio);

  if (soundFound == m_sounds.end()) //if sound not found, load sound
  {
    LoadSound(audio);
    soundFound = m_sounds.find(audio);
    if (soundFound == m_sounds.end()) //if sound still not found, return 
    {
      return;
    }
  }

  FMOD::Channel* channel = nullptr;
  if (isSFX)
  {
    channel = m_channels[m_SFX];
    m_playlist[audio] = m_SFX;
  }
  else
  {
    channel = m_channels[m_BGM];
    m_playlist[audio] = m_BGM;
  }

  ErrorCheck(m_fModSystem->playSound(soundFound->second, nullptr, true, &channel));
  if (channel)
  {
    ErrorCheck(channel->setPaused(false));
  }
}

void FmodSystem::PlayLoopedSound(std::string audio, bool isSFX)
{
  SoundMap::iterator soundFound = m_sounds.find(audio);

  if (soundFound == m_sounds.end()) //if sound not found, load sound
  {
    LoadLoopedSound(audio);
    soundFound = m_sounds.find(audio);
    if (soundFound == m_sounds.end()) //if sound still not found, return 
    {
      return;
    }
  }

  FMOD::Channel* channel = nullptr;
  if (isSFX)
  {
    channel = m_channels[m_SFX];
    m_playlist[audio] = m_SFX;
  }
  else
  {
    channel = m_channels[m_BGM];
    m_playlist[audio] = m_BGM;
  }

  ErrorCheck(m_fModSystem->playSound(soundFound->second, nullptr, true, &channel));
  if (channel)
  {
    ErrorCheck(channel->setPaused(false));
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

void FmodSystem::StopChannel(ChannelID channel)
{
  m_channels[channel]->stop();
}

void FmodSystem::SetChannelVolume(bool isSFX, double volumedB)
{
  if (isSFX)
  {
    ErrorCheck(m_channels[m_SFX]->setVolume(static_cast<float>(dbToVolume(volumedB))));
  }
  else
  {
    ErrorCheck(m_channels[m_BGM]->setVolume(static_cast<float>(dbToVolume(volumedB))));
  }
}

bool FmodSystem::IsPlaying(std::pair<std::string, ChannelID> audio)
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
    throw Debug::Exception<FmodSystem>(Debug::LEVEL_ERROR, ErrMsg(error.c_str()));
  }
}
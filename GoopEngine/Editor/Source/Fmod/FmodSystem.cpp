/*!*********************************************************************
\file   FmodSystem.cpp
\author c.phua\@digipen.edu
\date   8 November 2023
\brief
    Fmod system.
    Uses the fMOD library to create sounds and channels.
    There are four channels: BGM, SFX, Voice, TotalChannels.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Fmod/FmodSystem.h>
#include <fmod_errors.h>
#include <AssetManager/AssetManager.h>

using namespace GE::fMOD;

FmodSystem::~FmodSystem()
{
  std::for_each(m_channels.begin(), m_channels.end(), [](auto& s) { s.second->stop(); });
  m_channels.clear();
  std::for_each(m_sounds.begin(), m_sounds.end(), [](auto& s) { s.second->release(); });
  m_sounds.clear();
  // if fmod not initialized dont close 
  if (m_fModSystem != nullptr)
  {
    ErrorCheck(m_fModSystem->close());
    ErrorCheck(m_fModSystem->release());

    m_fModSystem = nullptr;
  }
}

void FmodSystem::Init()
{
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::WindowGainFocusEvent>(this);
  GE::Events::EventManager::GetInstance().Subscribe<GE::Events::WindowLoseFocusEvent>(this);

  ErrorCheck(FMOD::System_Create(&m_fModSystem)); // Create the FMOD Core system
  int maxSounds = GE::Assets::AssetManager::GetInstance().GetConfigData<int>("MaxPlayingSounds");
  ErrorCheck(m_fModSystem->init(maxSounds, FMOD_INIT_THREAD_UNSAFE | FMOD_INIT_STREAM_FROM_UPDATE, NULL)); // Initialize the FMOD Core system
  m_fModSystem->getMasterChannelGroup(&m_masterGroup);

  for (int i{}; i < TOTAL_CHANNELS; ++i)
  {
    ChannelType currChannel = static_cast<ChannelType>(i);
    FMOD::ChannelGroup*& channelGroup{m_channelGroups[currChannel]};
    ErrorCheck(m_fModSystem->createChannelGroup(m_channelToString.at(currChannel).c_str(), &channelGroup));
    // This should be set from config file
    //channelGroup->setVolume(1.0f);
    m_volumes[currChannel] = 1.0f;

    ErrorCheck(m_masterGroup->addGroup(m_channelGroups[currChannel]));
  }
}

void FmodSystem::Update()
{
  ErrorCheck(m_fModSystem->update());
  UnLoadSounds();

  double dt{ GE::FPS::FrameRateController::GetInstance().GetFixedDeltaTime() };
  for (size_t i{}; i < m_pausePlayRequests.size(); ++i)
  {
    PausePlayRequest& curr{ m_pausePlayRequests[i] };
    curr.timer += dt;
    float volumeFactor{ static_cast<float>((curr.pause ? FadeTime - curr.timer : curr.timer) / FadeTime) };
    float targetVolume { GetChannelVolume(curr.channel) };
    volumeFactor = (volumeFactor >= 1.f ? 1.f : volumeFactor);
    ErrorCheck(m_channelGroups[curr.channel]->setVolume(volumeFactor * targetVolume));
    if (curr.timer >= FadeTime) // is it time to actually set paused?
    {
      ErrorCheck(m_channelGroups[curr.channel]->setPaused(curr.pause));
      m_pausePlayRequests.erase(m_pausePlayRequests.begin() + i);
      continue; // we're done. Skip to next iteration
    }

  }
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

    val->isPlaying(&isPlaying);
    if (!isPlaying)
    {
      remove.push_back(key);
    }
  }

  for (auto& key : remove)
  {
    ErrorCheck(m_sounds[key]->release());
    m_channels.erase(key);
    m_sounds.erase(key);
  }  
}

bool FmodSystem::isPlaying(std::string audio)
{
  auto it = m_channels.find(audio);
  if (it == m_channels.end())
    return false;

  bool playing;
  (*it).second->isPlaying(&playing);
  return playing;
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
    ErrorCheck(m_fModSystem->playSound(soundFound->second, m_channelGroups[channel], true, &soundChannel));
    ErrorCheck(soundChannel->setVolumeRamp(true));
    ErrorCheck(soundChannel->setVolume(volume));
    ErrorCheck(soundChannel->setPaused(false));
  }
  else
  {
    bool isPaused{};
    soundChannel->getPaused(&isPaused);
    // if sound paused, unpause
    if (isPaused)
    {
      soundChannel->setPaused(false);
    }
    else
    {
      ErrorCheck(m_fModSystem->playSound(soundFound->second, m_channelGroups[channel], true, &soundChannel));
      ErrorCheck(soundChannel->setVolumeRamp(true));
      ErrorCheck(soundChannel->setVolume(volume));
      ErrorCheck(soundChannel->setPaused(false));
    }
  }
}

void GE::fMOD::FmodSystem::Pause(std::string audio)
{
  if (m_channels.find(audio) != m_channels.end())
  {
    ErrorCheck(m_channels[audio]->setPaused(true));
  }
}

float GE::fMOD::FmodSystem::GetVolume(std::string audio)
{
  float vol{};
  if (m_channels.find(audio) != m_channels.end())
  {
    ErrorCheck(m_channels[audio]->getVolume(&vol));
  }

  return vol;
}

void FmodSystem::StopSound(std::string audio)
{
  ErrorCheck(m_channels[audio]->stop());
}

void FmodSystem::StopAllSound()
{
  ErrorCheck(m_masterGroup->stop());
}

void FmodSystem::StopChannel(ChannelType channel)
{
  ErrorCheck(m_channelGroups[channel]->stop());
}

void GE::fMOD::FmodSystem::SetChannelPause(ChannelType channel, bool paused)
{
  bool currState;
  ErrorCheck(m_channelGroups[channel]->getPaused(&currState)); // get the old state

  if (paused == currState)
    return;

  for (size_t i{}; i < m_pausePlayRequests.size(); ++i)
  {
    PausePlayRequest const& req{ m_pausePlayRequests[i] };
    if (req.channel == channel)
    {
      m_pausePlayRequests.erase(m_pausePlayRequests.begin() + i);
      //if (currState == req.pause)
      //  return;
      return;
    }
  }

  m_pausePlayRequests.emplace_back(paused, channel);
  if (!paused) // must play the audio
    ErrorCheck(m_channelGroups[channel]->setPaused(false));
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
  ErrorCheck(m_channels[audio]->setVolume(volume));
}

void FmodSystem::SetMasterVolume(float volume)
{
  ErrorCheck(m_masterGroup->setVolume(volume));
}

float GE::fMOD::FmodSystem::GetMasterVolume() const
{
  float vol;
  ErrorCheck(m_masterGroup->getVolume(&vol));
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
    SetChannelPause(BGM, true);
    SetChannelPause(SFX, true);
    SetChannelPause(VOICE, true);
    //ErrorCheck(m_masterGroup->setPaused(true));
    break;
  case GE::Events::EVENT_TYPE::WINDOW_GAIN_FOCUS:
    //ErrorCheck(m_masterGroup->setPaused(false));
    SetChannelPause(BGM, false);
    SetChannelPause(SFX, false);
    SetChannelPause(VOICE, false);
    break;
  default:
    break;
  }
}

void FmodSystem::ErrorCheck(FMOD_RESULT result) const
{
  static GE::Debug::ErrorLogger& logger = GE::Debug::ErrorLogger::GetInstance();
  if (result != FMOD_OK)
  {
    std::string error = FMOD_ErrorString(result);
    logger.LogError<FmodSystem>(error);
  }
}
#include <pch.h>
#include "AudioEngine.h"

using namespace GE::Audio;

Implementation::Implementation() 
{
  AudioEngine::ErrorCheck(FMOD::System_Create(&fm_system)); // Create the FMOD Core system
  AudioEngine::ErrorCheck(fm_system->init(m_numOfChannels, FMOD_INIT_STREAM_FROM_UPDATE, NULL)); // Initialize the FMOD Core system
}


Implementation::~Implementation() 
{
  AudioEngine::ErrorCheck(fm_system->close());
}

void Implementation::Update() 
{
  std::vector<ChannelMap::iterator> stoppedChannels;
  for (auto start = m_channels.begin(), end = m_channels.end(); start != end; ++start)
  {
    bool isPlaying = false;
    start->second->isPlaying(&isPlaying);
    if (!isPlaying)
    {
        stoppedChannels.push_back(start);
    }
  }

  for (auto& itr : stoppedChannels)
  {
    m_channels.erase(itr);
  }
  AudioEngine::ErrorCheck(fm_system->update());
}

Implementation* fMOD = nullptr;

AudioEngine::AudioEngine()
{
  fMOD = new Implementation;
}

AudioEngine::~AudioEngine()
{
  delete fMOD;
}

void AudioEngine::Update() const
{
  fMOD->Update();
}

void AudioEngine::LoadSound(const std::string& soundFile, bool isLooping, bool isStreaming) const
{
  // Check if the sound is already loaded
  auto soundFound = fMOD->m_sounds.find(soundFile);
  if (soundFound != fMOD->m_sounds.end())
  {
    return;
  }

  FMOD_MODE mode = FMOD_DEFAULT;
  mode |= isLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
  mode |= isStreaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

  FMOD::Sound* sound = nullptr;
  
  // Use FMOD Core API to create the sound
  FMOD_RESULT result = fMOD->fm_system->createSound(soundFile.c_str(), mode, nullptr, &sound);
  if (result == FMOD_OK)
  {
    fMOD->m_sounds[soundFile] = sound;
  }
  else
  {
    std::ostringstream oss{};
    oss << "Failed to create sound: " << result;
    GE::Debug::ErrorLogger::GetInstance().LogError(oss.str());
  }
}

void AudioEngine::UnLoadSound(const std::string& strSoundName) const
{
  auto soundFound = fMOD->m_sounds.find(strSoundName);
  if (soundFound == fMOD->m_sounds.end())
  {
    return;
  }

  AudioEngine::ErrorCheck(soundFound->second->release());
  fMOD->m_sounds.erase(soundFound);
}

void AudioEngine::PlaySound(const std::string& sound, float volumedB, bool isLooping, bool isStreaming) const
{
#if 0
  int channelId = -1;
  Implementation::SoundMap::iterator soundFound = fMOD->m_sounds.find(sound);

  if (soundFound == fMOD->m_sounds.end()) //if sound not found, load sound
  {
    LoadSound(sound, isLooping, isStreaming);
    soundFound = fMOD->m_sounds.find(sound);
    if (soundFound == fMOD->m_sounds.end()) //if sound still not found, return 
    {
      return;
    }
  }

  // Search for an available channel in the channel pool
  for (const auto& entry : fMOD->m_playlist)
  {
    if (entry.first == sound)
    {
      channelId = entry.second;
      break;
    }
  }

  // If a channel is available, reuse it
  if (channelId != -1)
  {
    FMOD::Channel* channel = fMOD->m_channels[channelId];
    AudioEngine::ErrorCheck(channel->setPosition(0, FMOD_TIMEUNIT_MS)); // Seek to the beginning instead of playing again
  }

  FMOD::Channel* channel = nullptr;
  AudioEngine::ErrorCheck(fMOD->fm_system->playSound(soundFound->second, nullptr, true, &channel));
  if (channel)
  {
    channelId = fMOD->m_nextChannelId++;
    AudioEngine::ErrorCheck(channel->setVolume(dbToVolume(volumedB)));
    AudioEngine::ErrorCheck(channel->setPaused(false));
    fMOD->m_channels[channelId] = channel;
    fMOD->m_playlist[sound] = channelId;  // add song and channel to playlist
  }
#else
  int channelId = fMOD->m_nextChannelId++;
  Implementation::SoundMap::iterator soundFound = fMOD->m_sounds.find(sound);

  if (soundFound == fMOD->m_sounds.end()) //if sound not found, load sound
  {
    LoadSound(sound, isLooping, isStreaming);
    soundFound = fMOD->m_sounds.find(sound);
    if (soundFound == fMOD->m_sounds.end()) //if sound still not found, return 
    {
      return;
    }
  }

  FMOD::Channel* channel = nullptr;
  AudioEngine::ErrorCheck(fMOD->fm_system->playSound(soundFound->second, nullptr, true, &channel));
  if (channel)
  {
    AudioEngine::ErrorCheck(channel->setVolume(dbToVolume(volumedB)));
    AudioEngine::ErrorCheck(channel->setPaused(false));
    fMOD->m_channels[channelId] = channel;
    fMOD->m_playlist[sound] = channelId;  // add song and channel to playlist
  }
#endif
}

void AudioEngine::StopSound(const std::string& sound) const
{
  Implementation::ChannelID const& channel = fMOD->m_playlist[sound];
  fMOD->m_channels[channel]->stop();
  fMOD->m_playlist.erase(sound);
}

void AudioEngine::StopAllChannels() const
{
  for (int i{}; i < fMOD->m_channels.size(); ++i) {
    fMOD->m_channels[i]->stop();
  }
  fMOD->m_playlist.clear();
}

void AudioEngine::SetChannelVolume(int channelId, float volumedB) const
{
  auto channelFound = fMOD->m_channels.find(channelId);
  if (channelFound == fMOD->m_channels.end())
  {
    return;
  }

  AudioEngine::ErrorCheck(channelFound->second->setVolume(dbToVolume(volumedB)));
}

bool AudioEngine::IsPlaying(int channelId) const
{
  bool playingFlag{ false };
  fMOD->m_channels[channelId]->isPlaying(&playingFlag);
  if (playingFlag) {
    return true;
  }
  return false;
}

float AudioEngine::dbToVolume(float dB) const
{
  return powf(10.0f, 0.05f * dB);
}

float AudioEngine::VolumeTodb(float volume) const
{
  return 20.0f * log10f(volume);
}

void AudioEngine::ErrorCheck(FMOD_RESULT result)
{
  if (result != FMOD_OK) 
  {
    std::cout << "FMOD ERROR " << result << std::endl;
    return;
  }
}
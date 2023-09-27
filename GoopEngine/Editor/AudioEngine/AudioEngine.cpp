#include "AudioEngine.h"

using namespace GE::AudioEngine;

Implementation::Implementation() 
{
  fm_studioSystem = NULL;
  AudioEngine::ErrorCheck(FMOD::Studio::System::create(&fm_studioSystem));
  AudioEngine::ErrorCheck(fm_studioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

  fm_system = NULL;
  AudioEngine::ErrorCheck(fm_studioSystem->getCoreSystem(&fm_system));
}

Implementation::~Implementation() 
{
  AudioEngine::ErrorCheck(fm_studioSystem->unloadAll());
  AudioEngine::ErrorCheck(fm_studioSystem->release());
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
  AudioEngine::ErrorCheck(fm_studioSystem->update());
}

Implementation* fMOD = nullptr;

void AudioEngine::Init() 
{
  fMOD = new Implementation;
}

void AudioEngine::Update() 
{
  fMOD->Update();
}

void AudioEngine::LoadSound(const std::string& soundFile)
{
  auto soundFound = fMOD->m_sounds.find(soundFile);
  if (soundFound != fMOD->m_sounds.end())
  {
     return;
  }

  FMOD::Sound* sound = nullptr;
  AudioEngine::ErrorCheck(fMOD->fm_studioSystem->startCommandCapture(soundFile.c_str(), FMOD_STUDIO_COMMANDCAPTURE_NORMAL));
  if (sound)
  {
    fMOD->m_sounds[soundFile] = sound;
  }
}

void AudioEngine::UnLoadSound(const std::string& strSoundName)
{
  auto soundFound = fMOD->m_sounds.find(strSoundName);
  if (soundFound == fMOD->m_sounds.end())
  {
    return;
  }

  AudioEngine::ErrorCheck(soundFound->second->release());
  fMOD->m_sounds.erase(soundFound);
}

int AudioEngine::PlaySound(const std::string& sound, float volumedB)
{
  int channelId = fMOD->m_nextChannelId++;
  auto soundFound = fMOD->m_sounds.find(sound);
  if (soundFound == fMOD->m_sounds.end())
  {
    LoadSound(sound);
    soundFound = fMOD->m_sounds.find(sound);
    if (soundFound == fMOD->m_sounds.end())
    {
        return channelId;
    }
  }
  FMOD::Channel* channel = nullptr;
  AudioEngine::ErrorCheck(fMOD->fm_system->playSound(soundFound->second, nullptr, true, &channel));
  if (channel)
  {
    AudioEngine::ErrorCheck(channel->setVolume(dbToVolume(volumedB)));
    AudioEngine::ErrorCheck(channel->setPaused(false));
    fMOD->m_channels[channelId] = channel;
  }
  return channelId;
}

void AudioEngine::StopChannel(int channelId)
{
  fMOD->m_channels[channelId]->stop();
}

void AudioEngine::StopAllChannels()
{
  for (int i{}; i < fMOD->m_channels.size(); ++i) {
    fMOD->m_channels[i]->stop();
  }
}

void AudioEngine::SetChannelVolume(int channelId, float volumedB)
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

float AudioEngine::dbToVolume(float dB)
{
    return powf(10.0f, 0.05f * dB);
}

float AudioEngine::VolumeTodb(float volume)
{
    return 20.0f * log10f(volume);
}

int AudioEngine::ErrorCheck(FMOD_RESULT result) 
{
    if (result != FMOD_OK) 
    {
        std::cout << "FMOD ERROR " << result << std::endl;
        return 1;
    }
    //std::cout << "FMOD all good" << std::endl;
    return 0;
}

void AudioEngine::Shutdown() 
{
    delete fMOD;
}

void AudioEngine::AudioTest()
{
  Init();
  
}
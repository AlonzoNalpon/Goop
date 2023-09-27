#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include <fMOD/fmod_studio.hpp>
#include "fMOD/fmod.hpp"
#include <pch.h>
#include <string>
#include <map>
#include <iostream>

#endif

namespace GE 
{
	namespace AudioEngine 
	{
    //for fMOD constucting and destructing, etc.
    struct Implementation {
      Implementation();
      ~Implementation();
      void Update();

      FMOD::Studio::System* fm_studioSystem;
      FMOD::System* fm_system;

      int m_nextChannelId{};

      using SoundMap = std::map<std::string, FMOD::Sound*> ;
      using ChannelMap = std::map<int, FMOD::Channel*>;

      SoundMap m_sounds;
      ChannelMap m_channels;
    };

    //audio engine
    class AudioEngine {
    public:
      static void Init();
      static void Update();
      static void Shutdown();
      static int ErrorCheck(FMOD_RESULT result);
      void LoadSound(const std::string& soundFile);
      void UnLoadSound(const std::string& sound);
      int PlaySound(const std::string& sound, float volumedB = 0.0f);
      void StopChannel(int channelId);
      void StopAllChannels();
      void SetChannelVolume(int channelId, float volumedB);
      bool IsPlaying(int channelId) const;
      float dbToVolume(float db);
      float VolumeTodb(float volume);

      static void AudioTest();
    };
	}
}
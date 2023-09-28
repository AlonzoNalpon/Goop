#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include <fmod.hpp>
#include <pch.h>
#include <string>
#include <map>
#include <iostream>

#endif

namespace GE 
{
	namespace Audio
	{
    //for fMOD constucting and destructing, etc.
    struct Implementation {
      using ChannelID = unsigned;

      Implementation();
      ~Implementation();
      void Update();

      FMOD::System* fm_system{ NULL };

      ChannelID m_nextChannelId{};
      ChannelID m_numOfChannels{ 32 };

      using SoundMap = std::map<std::string, FMOD::Sound*> ;
      using ChannelMap = std::map<ChannelID, FMOD::Channel*>;
      using CurrentPlaylist = std::unordered_map<std::string, ChannelID>;

      SoundMap m_sounds;
      ChannelMap m_channels;
      CurrentPlaylist m_playlist;
    };

    //audio engine
    class AudioEngine : public Singleton<AudioEngine> {
    public:
      static void Init();
      static void Update();
      static void Shutdown();
      static int ErrorCheck(FMOD_RESULT result);
      void LoadSound(const std::string& soundFile, bool isLooping, bool isStreaming);
      void UnLoadSound(const std::string& sound);
      void PlaySound(const std::string& sound, float volumedB = 0.0f, bool isLooping = false, bool isStreaming = true);
      void StopChannel(std::string&& sound);
      void StopAllChannels();
      void SetChannelVolume(int channelId, float volumedB);
      bool IsPlaying(int channelId) const;
      float dbToVolume(float db);
      float VolumeTodb(float volume);

      //static void AudioTest();
    };
	}
}
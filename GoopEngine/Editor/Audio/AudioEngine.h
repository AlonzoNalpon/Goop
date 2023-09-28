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

      FMOD::System* fm_system{ nullptr };

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
      void Init() const;
      void Update() const;
      void Shutdown() const;
      void PlaySound(const std::string& sound, float volumedB = 0.0f, bool isLooping = false, bool isStreaming = true) const;
      void StopSound(std::string&& sound) const;
      void StopAllChannels() const;
      void SetChannelVolume(int channelId, float volumedB) const;

      static int ErrorCheck(FMOD_RESULT result);

    private:
      void LoadSound(const std::string& soundFile, bool isLooping, bool isStreaming) const;
      void UnLoadSound(const std::string& sound) const;
      bool IsPlaying(int channelId) const;
      float dbToVolume(float db) const;
      float VolumeTodb(float volume) const;
    };
	}
}
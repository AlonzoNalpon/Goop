/*!*********************************************************************
\file   Audio.h
\author c.phua\@digipen.edu
\date   8 November 2023
\brief
  Integrates fMOD into an Audio Component.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <fmod.hpp>
#include <Singleton/Singleton.h>

namespace GE 
{
	namespace fMOD
	{
    class FmodSystem : public Singleton<FmodSystem> 
    {
    public:
      enum ChannelType
      {
        BGM,
        SFX,
        VOICE,
        TOTAL_CHANNELS,
      };

      static inline const std::map<ChannelType, std::string> m_channelToString
      {
        { BGM, "BGM"},
        { SFX, "SFX" },
        { VOICE, "VOICE" },
      };

      /*!*********************************************************************
      \brief
        Default destructor
      ************************************************************************/
      ~FmodSystem();

      /*!*********************************************************************
      \brief
        Initializes the system
      ************************************************************************/
      void Init();

      /*!*********************************************************************
      \brief
        Erases any channels that are not playing any sounds and updates fMOD system.
      ************************************************************************/
      void Update();

      /*!*********************************************************************
      \brief
        Loads sound into a new channel and plays it. Loads sound if not in sound map already.
      \param sound
        Sound filename.
      \param volumedB
        Volume in db. Default: 0.0f.
      \param isLooping
        Boolean for looping the sound. Default: false.
      \param isStreaming
        Boolean for streaming the sound. Default: true.
      ************************************************************************/
      void PlaySound(std::string audio, ChannelType channel, bool looped = false);

      void Pause(std::string audio);

      /*!*********************************************************************
      \brief
        Modifies channel's volume.
      \param channelID
        ID of the channel.
      \param volumedB
        Volume in db.
      ************************************************************************/
      void SetChannelVolume(ChannelType channel, float volume);

      float GetChannelVolume(ChannelType channel) const;

      void SetMasterVolume(float volume);

      float GetMasterVolume() const;

      /*!*********************************************************************
      \brief
        Stops a single channel that has the sound being passed in.
      \param sound
        Sound filename.
      ************************************************************************/
      void StopSound(std::string audio);

      void StopAllSound();

      void StopChannel(ChannelType channel);

    private:
      FMOD::System* m_fModSystem{ nullptr };

      using SoundMap = std::map<std::string, FMOD::Sound*>;
      using SoundChannel = std::map<std::string, FMOD::Channel*>;
      using ChannelGroups = std::map<ChannelType, FMOD::ChannelGroup*>;

      FMOD::ChannelGroup* m_masterGroup;
      SoundMap m_sounds;
      ChannelGroups m_channelGroups;
      SoundChannel m_channels;
      std::map<ChannelType, float> m_volumes;

      /*!*********************************************************************
      \brief
        Prints out error message into console if there is an error.
      \param result
        Result of fMOD functions.
      ************************************************************************/
      void ErrorCheck(FMOD_RESULT result);

      /*!*********************************************************************
      \brief
        Loads sound into sound map if not already inside.
      \param audio
        Sound filename.
      \param looped
        Boolean for if the sound is looping.

      \return
        True if audio loaded succesfully
        False if audio failed to load
      ************************************************************************/
      bool LoadSound(std::string audio, bool looped = false);

      /*!*********************************************************************
      \brief
        This function is depreciated.
        Unload channels which the sounds are no longer playing.
      ************************************************************************/
      void UnLoadSounds();

      /*!*********************************************************************
      \brief
        Converts db to volume.
      \param db
        Sound volume in db.
      \return float
        Volume converted from db.
      ************************************************************************/
      float dbToVolume(float db) const;

      /*!*********************************************************************
      \brief
        Converts volume to db.
      \param volume
        Sound volume.
      \return flaot
        db converted from volume.
      ************************************************************************/
      float VolumeTodb(float volume) const;
    };
	}
}
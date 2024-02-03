/*!*********************************************************************
\file   FmodSystem.h
\author c.phua\@digipen.edu
\date   8 November 2023
\brief
    Fmod system.
    Uses the fMOD library to create sounds and channels.
    There are four channels: BGM, SFX, Voice, TotalChannels.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <fmod.hpp>
#include <Singleton/Singleton.h>

namespace GE 
{
	namespace fMOD
	{
    class FmodSystem : public Singleton<FmodSystem>, public GE::Events::IEventListener
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
      \param audio
        Sound filename.
      \param volume
        Normalized volume
      \param channel
        Channel the sound is controlled by
      \param looped
        Boolean for looping the sound. Default: false.
      ************************************************************************/
      void PlaySound(std::string audio, float volume, ChannelType channel = SFX, bool looped = false);

      /*!*********************************************************************
      \brief
        Pauses the audio.
      \param audio
        Audio file name.
      ************************************************************************/
      void Pause(std::string audio);

      /*!*********************************************************************
      \brief
        Sets channel's volume.
      \param channel
        The channel to set the volume of.
      \param volume
        Volume.
      ************************************************************************/
      void SetChannelVolume(ChannelType channel, float volume);

      /*!*********************************************************************
      \brief
        Gets the channel's current volume.
      \param channel
        The channel to get the volume from.
      \return float
        Returns the volume of the channel.
      ************************************************************************/
      float GetChannelVolume(ChannelType channel) const;

      /*!*********************************************************************
      \brief
        Sets audio's volume.
      \param audio
        The audio to set the volum of.
      \param volume
        Volume.
      ************************************************************************/
      void SetVolume(std::string audio, float volume);

      /*!*********************************************************************
      \brief
        Sets all channel's volume.
      \param volume
        Volume.
      ************************************************************************/
      void SetMasterVolume(float volume);

      /*!*********************************************************************
      \brief
        Gets the master volume.
      \return float
        Returns the volume of the channel.
      ************************************************************************/
      float GetMasterVolume() const;

      /*!*********************************************************************
      \brief
        Gets the audio of a specific sound if its playing

      \param audio
        Name of audio

      \return
        Volume of the playing audio or 0.f otherwise
      ************************************************************************/
      float GetVolume(std::string audio);

      /*!*********************************************************************
      \brief
        Stops a single channel that has the sound being passed in.
      \param sound
        Sound filename.
      ************************************************************************/
      void StopSound(std::string audio);

      /*!*********************************************************************
      \brief
        Stops all sounds and channels.
      ************************************************************************/
      void StopAllSound();

      /*!*********************************************************************
      \brief
        Stops the channel.
      \param channel
        The channel to stop.
      ************************************************************************/
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

      /*!*********************************************************************
      \brief
        Handles events happening to the audio system.
      \param event
        The current event happening.
      ************************************************************************/
      void HandleEvent(GE::Events::Event* event);
    };
	}
}
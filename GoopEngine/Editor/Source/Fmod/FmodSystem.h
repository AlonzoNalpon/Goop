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
      enum Channel
      {
        BGM,
        SFX,
        VOICE,
        TOTAL_CHANNELS,
      };

      static inline const std::map<Channel, std::string> m_channelToString
      {
        { BGM, "BGM"},
        { SFX, "SFX" },
        { VOICE, "VOICE" },
      };

      /*!*********************************************************************
      \brief
        Default contructor
      ************************************************************************/
      FmodSystem();

      /*!*********************************************************************
      \brief
        Default destructor
      ************************************************************************/
      ~FmodSystem();

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
      void PlaySound(std::string audio, Channel channel, bool looped = false);

      /*!*********************************************************************
      \brief
        Modifies channel's volume.
      \param channelID
        ID of the channel.
      \param volumedB
        Volume in db.
      ************************************************************************/
      void SetChannelVolume(Channel channel, double volumedB);

      /*!*********************************************************************
      \brief
        Stops a single channel that has the sound being passed in.
      \param sound
        Sound filename.
      ************************************************************************/
      void StopSound(std::string audio);

      void StopAllSound();

      void StopChannel(Channel channel);

    private:
      FMOD::System* m_fModSystem{ nullptr };

      using SoundMap = std::map<std::string, FMOD::Sound*>;
      using ChannelMap = std::map<Channel, FMOD::Channel*>;
      using CurrentPlaylist = std::unordered_map<std::string, Channel>;

      SoundMap m_sounds;
      ChannelMap m_channels;
      CurrentPlaylist m_playlist;

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
      ************************************************************************/
      void LoadSound(std::string audio, bool looped = false);

      /*!*********************************************************************
      \brief
        Loops through whole playlist and finds sounds that have stopped playing.
        Unloads sound that has stopped playing from sound map.
      ************************************************************************/
      void UnLoadSound();

      /*!*********************************************************************
      \brief
        Checks if channel is currently playing or not.
      \param channelId
        ID for channel.
      \return bool
        Returns true if channel is currently playing.
        Returns false if channel is not currently playing.
      ************************************************************************/
      bool IsPlaying(std::pair<std::string, Channel> audio);

      /*!*********************************************************************
      \brief
        Converts db to volume.
      \param db
        Sound volume in db.
      \return float
        Volume converted from db.
      ************************************************************************/
      double dbToVolume(double db) const;

      /*!*********************************************************************
      \brief
        Converts volume to db.
      \param volume
        Sound volume.
      \return flaot
        db converted from volume.
      ************************************************************************/
      double VolumeTodb(double volume) const;
    };
	}
}
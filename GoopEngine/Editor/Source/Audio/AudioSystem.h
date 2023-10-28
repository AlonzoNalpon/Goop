/*!*********************************************************************
\file   AudioSystem.h
\author c.phua\@digipen.edu
\date   28 September 2023
\brief
  Integrates fMOD into an Audio Engine.
  Loads, creates, play and stop sounds.

Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include <fmod.hpp>

namespace GE 
{
	namespace Systems
	{
    //audio engine
    class AudioSystem : public Singleton<AudioSystem> {
    public:
      /*!*********************************************************************
      \brief
        Default contructor
      ************************************************************************/
      AudioSystem();

      /*!*********************************************************************
      \brief
        Default destructor
      ************************************************************************/
      ~AudioSystem();

      /*!*********************************************************************
      \brief
        Runs fMOD's update function from Implementation struct.
      ************************************************************************/
      void Update() const;

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
      void PlaySound(const std::string& sound, float volumedB = 0.0f, bool isLooping = false, bool isStreaming = true) const;

      /*!*********************************************************************
      \brief
        Stops a single channel that has the sound being passed in.
      \param sound
        Sound filename.
      ************************************************************************/
      void StopSound(const std::string& sound) const;

      /*!*********************************************************************
      \brief
        Stops all channels.
      ************************************************************************/
      void StopAllChannels() const;

      /*!*********************************************************************
      \brief
        Modifies channel's volume.
      \param channelID
        ID of the channel.
      \param volumedB
        Volume in db.
      ************************************************************************/
      void SetChannelVolume(int channelId, float volumedB) const;

      /*!*********************************************************************
      \brief
        Prints out error message into console if there is an error.
      \param result
        Result of fMOD functions.
      ************************************************************************/
      static void ErrorCheck(FMOD_RESULT result);

    private:
      /*!*********************************************************************
      \brief
        Loads sound into sound map if not already inside.
      \param soundFile
        Sound filename.
      \param isLooping
        Boolean for looping the sound.
      \param isStreaming
        Boolean for streaming the sound.
      ************************************************************************/
      void LoadSound(const std::string& soundFile, bool isLooping, bool isStreaming) const;

      /*!*********************************************************************
      \brief
        Unloads sound from sound map.
      \param sound
        Sound filename.
      ************************************************************************/
      void UnLoadSound(const std::string& sound) const;

      /*!*********************************************************************
      \brief
        Checks if channel is currently playing or not.
      \param channelId
        ID for channel.
      \return bool
        Returns true if channel is currently playing.
        Returns false if channel is not currently playing.
      ************************************************************************/
      bool IsPlaying(int channelId) const;

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

#endif
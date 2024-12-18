/*!*********************************************************************
\file   Audio.h
\author c.phua\@digipen.edu
\date   8-November-2023
\brief
	Component for audio.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <Fmod/FmodSystem.h>
#include <Utilities/GoopUtils.h>

namespace GE
{
	namespace Component
	{
		struct Audio
		{
			struct Sound
			{
				float m_volume{1.0};
				std::string m_sound;

				bool m_loop = false;
				bool m_playOnStart = false;
				bool m_paused = false;

				bool m_playedOnStart = false;
				bool m_lastPausedState = m_paused;
				GE::fMOD::FmodSystem::ChannelType m_channel{GE::fMOD::FmodSystem::ChannelType::BGM};
			};
			std::vector<Sound> m_sounds;
		
			/*!*********************************************************************
			\brief
			  Plays all sounds in the component
			************************************************************************/
			void Play()
			{
				for (auto sound : m_sounds)
				{
					sound.m_paused = false;
					sound.m_lastPausedState = sound.m_paused;
					GE::fMOD::FmodSystem::GetInstance().PlaySound(sound.m_sound, sound.m_volume, sound.m_channel, sound.m_loop);
				}
			}

			/*!*********************************************************************
			\brief
				Plays a random sound in the component
			************************************************************************/
			void PlayRandom()
			{
				int index = GE::GoopUtils::RandomValue(0, static_cast<int>(m_sounds.size() - 1));
				auto& sound{ m_sounds[index] };
				sound.m_paused = false;
				sound.m_lastPausedState = sound.m_paused;
				GE::fMOD::FmodSystem::GetInstance().PlaySound(sound.m_sound, sound.m_volume, sound.m_channel, sound.m_loop);
			}

			/*!*********************************************************************
			\brief
			  Plays a random sound in the component given a range
			\param startRange
				The start of the range of sounds to play
			\param endRange
				The end range of sounds to play
			\param volume
				The volume to play the sound at
			************************************************************************/
			void PlayRandom(int startRange, int endRange, float volume = 1.f)
			{
				int index = GE::GoopUtils::RandomValue(startRange, endRange);
				auto& sound{ m_sounds[index] };
				sound.m_paused = false;
				sound.m_lastPausedState = sound.m_paused;
				GE::fMOD::FmodSystem::GetInstance().PlaySound(sound.m_sound, volume, sound.m_channel, sound.m_loop);
			}

			/*!*********************************************************************
			\brief
				Plays a sound
			************************************************************************/
			void Play(std::string soundName)
			{
				for (auto sound : m_sounds)
				{
					if (sound.m_sound != soundName)
					{
						continue;
					}

					sound.m_paused = false;
					sound.m_lastPausedState = sound.m_paused;
					GE::fMOD::FmodSystem::GetInstance().PlaySound(sound.m_sound, sound.m_volume, sound.m_channel, sound.m_loop);
					break;
				}
			}

			/*!*********************************************************************
			\brief
				Plays all sounds in the component
			************************************************************************/
			void Stop()
			{
				for (auto sound : m_sounds)
				{
					sound.m_paused = true;
					sound.m_lastPausedState = sound.m_paused;
					GE::fMOD::FmodSystem::GetInstance().StopSound(sound.m_sound);
				}
			}

			/*!*********************************************************************
			\brief
				Stops a sound
			************************************************************************/
			void Stop(std::string soundName)
			{
				for (auto sound : m_sounds)
				{
					if (sound.m_sound != soundName)
					{
						continue;
					}

					sound.m_paused = true;
					sound.m_lastPausedState = sound.m_paused;
					GE::fMOD::FmodSystem::GetInstance().StopSound(sound.m_sound);
					break;
				}
			}

			/*!*********************************************************************
			\brief
				Pause all sounds in the component
			************************************************************************/
			void Pause()
			{
				for (auto sound : m_sounds)
				{
					sound.m_paused = true;
					sound.m_lastPausedState = sound.m_paused;
					GE::fMOD::FmodSystem::GetInstance().Pause(sound.m_sound);
				}
			}

			/*!*********************************************************************
			\brief
				Pauses a sound
			************************************************************************/
			void Pause(std::string soundName)
			{
				for (auto sound : m_sounds)
				{
					if (sound.m_sound != soundName)
					{
						continue;
					}

					sound.m_paused = true;
					sound.m_lastPausedState = sound.m_paused;
					GE::fMOD::FmodSystem::GetInstance().Pause(sound.m_sound);
					break;
				}
			}
		};
	}
}
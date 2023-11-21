#pragma once
#include <Fmod/FmodSystem.h>

namespace GE
{
	namespace Component
	{
		struct Audio
		{
			std::string m_name = {""};
			bool m_loop = false;
			bool m_playOnStart = false;
			bool m_paused = false;

			bool m_playedOnStart = false;
			bool m_lastPausedState = m_paused;
			GE::fMOD::FmodSystem::ChannelType channel{GE::fMOD::FmodSystem::ChannelType::BGM};
		
			void Play()
			{
				m_paused = false;
				m_lastPausedState = m_paused;
				GE::fMOD::FmodSystem::GetInstance().PlaySound(m_name, channel, m_loop);
			}

			void Stop()
			{
				m_paused = true;
				m_lastPausedState = m_paused;
				GE::fMOD::FmodSystem::GetInstance().StopSound(m_name);
			}

			void Pause()
			{
				m_paused = true;
				m_lastPausedState = m_paused;
				GE::fMOD::FmodSystem::GetInstance().Pause(m_name);
			}
		};
	}
}
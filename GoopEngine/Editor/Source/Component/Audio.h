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
			bool m_initialized = false;
			bool m_playOnStart = false;
			bool m_isPlaying = false;
			GE::fMOD::FmodSystem::ChannelType channel{GE::fMOD::FmodSystem::ChannelType::BGM};
		
			void Play()
			{
				GE::fMOD::FmodSystem::GetInstance().PlaySound(m_name, channel, m_loop);
			}

			void Stop()
			{
				GE::fMOD::FmodSystem::GetInstance().StopSound(m_name);
			}
		};
	}
}
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
			GE::fMOD::FmodSystem::Channel channel{GE::fMOD::FmodSystem::Channel::BGM};
		};
	}
}
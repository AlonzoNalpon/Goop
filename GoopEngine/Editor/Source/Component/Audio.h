#pragma once

namespace GE
{
	namespace Component
	{
		struct Audio
		{
			std::string m_name = {};
			double m_volume = 1.0;
			bool m_loop = false;
			bool m_stream = true;
			bool m_play = true;
			bool m_isPlaying = false;
			bool m_isSFX;
		};
	}
}
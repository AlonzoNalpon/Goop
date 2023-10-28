#pragma once
#include <fmod.hpp>

namespace GE
{
  namespace Systems
  {
    struct Implementation {
      using ChannelID = unsigned;

      /*!*********************************************************************
      \brief
        Default contructor
      ************************************************************************/
      Implementation();

      /*!*********************************************************************
      \brief
        Default destructor
      ************************************************************************/
      ~Implementation();

      /*!*********************************************************************
      \brief
        Erases any channels that are not playing any sounds and updates fMOD system.
      ************************************************************************/
      void Update();

      FMOD::System* fm_system{ nullptr };

      ChannelID m_nextChannelId{};
      ChannelID m_numOfChannels{ 32 };

      using SoundMap = std::map<std::string, FMOD::Sound*>;
      using ChannelMap = std::map<ChannelID, FMOD::Channel*>;
      using CurrentPlaylist = std::unordered_map<std::string, ChannelID>;

      SoundMap m_sounds;
      ChannelMap m_channels;
      CurrentPlaylist m_playlist;
    };
  }
}
/*!*********************************************************************
\file   SpriteData.h
\author chengen.lau\@digipen.edu
\date   29-September-2023
\brief  Definition of SpriteData struct to encapsulate data of an
        animation sprite / spritesheet. This is used during the
        deserialization process to hand values over to the asset
        manager.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#pragma once
#include <string>

namespace GE
{
  namespace Serialization
  {
    struct SpriteData
    {
      std::string m_id;
      std::string m_filePath;
      unsigned m_slices, m_stacks;
      unsigned m_frames;
      double m_speed;
      u32    m_flags; //!< looping, etc.

      SpriteData() : m_id{}, m_filePath{}, m_slices{}, m_stacks{}, m_frames{}, m_speed() {}
      // Non-default ctor
      SpriteData(std::string const& id, std::string const& filePath,
        unsigned slices, unsigned stacks, unsigned frames, double speed, u32 flags) :
        m_id{ id }, m_filePath{ filePath }, m_slices{ slices },
        m_stacks{ stacks }, m_frames{ frames }, m_speed{ speed }, m_flags{ flags } {}
    };
  }
}

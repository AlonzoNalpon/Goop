#pragma once
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif

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

      SpriteData() : m_id{}, m_filePath{}, m_slices{}, m_stacks{}, m_frames{} {}
      // Non-default ctor
      SpriteData(std::string const& id, std::string const& filePath,
        unsigned slices, unsigned stacks, unsigned frames) :
        m_id{ id }, m_filePath{ filePath }, m_slices{ slices },
        m_stacks{ stacks }, m_frames{ frames } {}
    };
  }
}

#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H
#include <Graphics/SpriteData.h>
namespace GE::Component
{
  struct Sprite
  {
    Graphics::SpriteData m_spriteData; //!< composed of sprite data
  };
}
#endif

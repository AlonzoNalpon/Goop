#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H
#include <Graphics/SpriteData.h>
namespace GE::Component
{
  struct Sprite
  {
    Graphics::SpriteData spriteData; //!< composed of sprite data

    inline GLuint GetTextureHandle() const noexcept { return spriteData.texture; }
  };
}
#endif

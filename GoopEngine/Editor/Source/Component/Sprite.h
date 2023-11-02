#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H
#include <Graphics/SpriteData.h>
#include <Graphics/GraphicsEngine.h>
namespace GE::Component
{
  struct Sprite
  {
    Sprite() = default;
    Sprite(GLuint textureId)
    {
      m_spriteData.texture = textureId;
      auto const& textureObj = Graphics::GraphicsEngine::GetInstance().textureManager.GetTexture(textureId);
      m_spriteData.info.height = textureObj.height;
      m_spriteData.info.width = textureObj.width;
    }

    Graphics::SpriteData m_spriteData; //!< composed of sprite data
  };
}
#endif

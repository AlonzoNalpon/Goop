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
      auto const& gEngine{ Graphics::GraphicsEngine::GetInstance() };
      m_spriteData.texture = textureId;
      m_spriteName = gEngine.textureManager.GetTextureName(textureId);
      auto const& textureObj = gEngine.textureManager.GetTexture(textureId);
      m_spriteData.info.height = textureObj.height;
      m_spriteData.info.width = textureObj.width;
    }

    Graphics::SpriteData m_spriteData; //!< composed of sprite data
    std::string m_spriteName; //!< name of sprite
  };
}
#endif

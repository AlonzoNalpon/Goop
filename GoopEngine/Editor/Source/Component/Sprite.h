/*!*********************************************************************
\file   Sprite.h
\author a.nalpon\@digipen.edu
\date   2-November-2023
\brief  this file contains the definition of sprite component for ECS
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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

    Sprite(Graphics::SpriteData const& spriteData, std::string spriteName) : 
      m_spriteData{ spriteData }, m_spriteName{std::move(spriteName)}
    {
      m_spriteData.texture = Graphics::GraphicsEngine::GetInstance().textureManager.GetTextureID(m_spriteName);
      std::cout << "Sprite ctor called\n";
    }

    Graphics::SpriteData m_spriteData; //!< composed of sprite data
    std::string m_spriteName; //!< name of sprite
  };
}
#endif

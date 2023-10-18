/*!*********************************************************************
\file   TextureManager.cpp
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the texture manager
class.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Graphics/TextureManager.h>
#include <DebugTools/Exception/Exception.h>
namespace Graphics
{

  gObjID TextureManager::AddTexture(std::string const& name, GLint w, GLint h, unsigned char const* imageData)
  {
    Texture newTexture{};
    newTexture.height = h;
    newTexture.width = w;

    // Create the texture object ...
    glCreateTextures(GL_TEXTURE_2D, 1, &newTexture.textureHandle);
    // allocate GPU storage for texture image data loaded from file
    glTextureStorage2D(newTexture.textureHandle, 1, GL_RGBA8, w, h);
    // copy image data from client memory to GPU texture buffer memory
    glTextureSubImage2D(newTexture.textureHandle, 0, 0, 0, w, h,
      GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<GLfloat const*>(imageData));

    gObjID id{ m_textures.size() };
    m_textures.emplace_back(newTexture); // add this texture to texture container

    // Add the texture to the lookup table
    if (m_texturesLT.find(name) != m_texturesLT.end())
    {
      throw GE::Debug::Exception<TextureManager>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("texture of this name already exists: " + name));
    }
    m_texturesLT[name] = id;
    return id;
  }

  Texture const& TextureManager::GetTexture(gObjID id) const
  {
    return m_textures[id];
  }

  gObjID TextureManager::GetTextureID(std::string const& name) const
  {
    if (m_texturesLT.find(name) == m_texturesLT.end())
    {
      throw GE::Debug::Exception<TextureManager>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("No such texture found: " + name));
      //return BAD_OBJ_ID;
    }
    return m_texturesLT.at(name);
  }

}
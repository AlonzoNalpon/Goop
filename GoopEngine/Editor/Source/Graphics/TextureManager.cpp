/*!*********************************************************************
\file   TextureManager.cpp
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the texture manager
class.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Graphics/TextureManager.h>
#include <DebugTools/Exception/Exception.h>
namespace GE::Graphics
{

  GLuint TextureManager::AddTexture(std::string const& name, GLint w, GLint h, unsigned char const* imageData)
  {
    if (m_texturesLT.find(name) != m_texturesLT.end())
    {
      DestroyTexture(m_texturesLT[name]);
      GE::Debug::ErrorLogger::GetInstance().LogMessage(
        "TextureManager: Texture " + name + " already exists, replacing texture.");
      // return m_texturesLT[name];
      /*throw GE::Debug::Exception<TextureManager>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("texture of this name already exists: " + name));*/
    }
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

    m_textures.emplace(newTexture.textureHandle, newTexture); // add this texture to texture container

    // Add the texture to the lookup table
    m_texturesLT[name] = newTexture.textureHandle;
    m_texNameLT[newTexture.textureHandle] = name;
    return newTexture.textureHandle;
  }

  void TextureManager::DestroyTexture(GLuint textureID)
  {
    auto it = m_textures.find(textureID);
    if (it == m_textures.end())
      throw GE::Debug::Exception<TextureManager>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("Attempting to delete nonexistent texture of ID: " + textureID));
    
    {
      // Unbind the texture in case it's binded
      glBindTexture(GL_TEXTURE_2D, 0);

      // Delete the texture
      glDeleteTextures(1, &textureID);
    }

    // Clear from all 3 maps
    m_textures.erase(it);
    m_texturesLT.erase(m_texturesLT.find(m_texNameLT.at(textureID)));
    m_texNameLT.erase(textureID);
  }

  Texture const& TextureManager::GetTexture(GLuint id) const
  {
    auto it = m_textures.find(id);
    if (it == m_textures.end())
      throw GE::Debug::Exception<TextureManager>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("Unable to find texture of ID: " + id));
    return it->second;
  }

  GLuint TextureManager::GetTextureID(std::string const& name) const
  {
    if (m_texturesLT.find(name) == m_texturesLT.end())
    {
      throw GE::Debug::Exception<TextureManager>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("No such texture found: " + name));
    }
    return m_texturesLT.at(name);
  }

  std::string TextureManager::GetTextureName(GLuint id) const
  {
    if (m_texNameLT.find(id) == m_texNameLT.end())
    {
      throw GE::Debug::Exception<TextureManager>(GE::Debug::LEVEL_CRITICAL,
        ErrMsg("No such texture id found: " + id));
    }
    return m_texNameLT.at(id);
  }

  std::map<std::string, GLuint> const& TextureManager::GetTextureLT() const
  {
    return m_texturesLT;
  }

  void TextureManager::FreeTextures()
  {
    for (auto const& texIt : m_textures)
    {
      glDeleteTextures(1, &texIt.first);
    }
    m_textures.clear();
    m_texturesLT.clear();
    m_texNameLT.clear();
  }

}
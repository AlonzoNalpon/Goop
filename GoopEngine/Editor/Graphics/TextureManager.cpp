#include <Graphics/TextureManager.h>
#include <DebugTools/Exception/Exception.h>
namespace Graphics
{

  gObjID TextureManager::AddTexture(std::string const& name, GLint w, GLint h, unsigned char const* imageData)
  {
    Texture newTexture{};

    glCreateTextures(GL_TEXTURE_2D, 1, &newTexture.textureHandle);
    // allocate GPU storage for texture image data loaded from file
    glTextureStorage2D(newTexture.textureHandle, 1, GL_RGBA8, w, h);
    // copy image data from client memory to GPU texture buffer memory
    glTextureSubImage2D(newTexture.textureHandle, 0, 0, 0, w, h,
      GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<GLfloat const*>(imageData));

    gObjID id{ m_textures.size() };
    m_textures.emplace_back(newTexture);

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
    }
    return m_texturesLT.at(name);
  }

}
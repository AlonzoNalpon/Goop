#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Texture.h>
#include <vector>
#include <map>
// a better alternative to automatically storing and deleting textures coming soon
namespace Graphics {
  class TextureManager{
  public:
    gObjID AddTexture(std::string const& name, GLint w, GLint h, unsigned char const* imageData);
    void DestroyTextures();

    Texture const& GetTexture(gObjID id) const;
    gObjID GetTextureID(std::string const& name) const;
  private:
    std::vector<Texture> m_textures;
    std::map<std::string, gObjID> m_texturesLT;
  };
}
#endif

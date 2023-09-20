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
    gObjID AddTexture(GLint w, GLint h, unsigned char const* imageData);
    void DestroyTextures();

    Texture const& GetTexture(gObjID id) const;
  private:
    std::vector<Texture> m_textures;
  };
}
#endif

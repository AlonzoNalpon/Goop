#ifndef TEXTURE_H
#define TEXTURE_H
#include <Def.h>
/*!
 * \brief  
 * This class contains a texture handle to a texture object.
 * The class is also in charge of initializing texture objects
 * stored in the GPU.
 */
namespace Graphics {

  class Texture
  {
  public:
    Texture(std::string const& imagePath);
    ~Texture();
  private:
    GLuint m_texture; // the handle to the texture
  };
}

#endif
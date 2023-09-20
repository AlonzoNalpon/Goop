#ifndef TEXTURE_H
#define TEXTURE_H
#include <Def.h>
/*!
 * \brief  
 * This class contains a texture handle to a texture object, along with other attributes
 * The class is also in charge of allocation/deallocation of texture objects
 * stored in the GPU.
 */
namespace Graphics {

  struct Texture
  {
    GLuint textureHandle;         //!< the handle to the texture
    GLenum filtering;       //!< the filtering mode for the image
    GLenum format;          //!< format of data (RGBA)
    GLenum internalFormat;  //!< layout of pixel data in GPU(RGBA8)
    GLuint width, height;     //!< dimensions of the image
  };
}

#endif
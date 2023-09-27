#ifndef SPRITE_SUB_DATA
#define SPRITE_SUB_DATA

#include <Graphics/Def/GraphicsTypes.h>
namespace Graphics {
  /*!
   * \struct SpriteData
   * \brief
   * Contains sprite texcoords and dimensions but NOT texture ID (to be used by sprite animator and spriteData struct)
   */
  struct SpriteSubData {
    gVec2     texCoords;      //!< texture coordinates of sprite (important for tex atlas support)
    gVec2     texDims;        //!< texture dimensions of a sprite in float ranging from [0,1] (for shader uniforms)
    GLuint    width, height;  //!< integral sprite pixel dimensions in width and height (x and y) (can be useful in the future)
    SpriteSubData(gVec2 _texCoords, gVec2 _texDims, GLuint _width, GLuint _height) : 
      texCoords{ _texCoords }, texDims{ _texDims }, width{ _width }, height{ _height } {}
    SpriteSubData() : texCoords{}, texDims{1, 1}, width{}, height{} {}
    // It is possible that sprite data can have varying sizes which requires offsets. 
    // This is the place to put offset if it's to be implemented (in pixels based on dimensions).
    // pivot offset should be done in model space first before game object transform
  };
}


#endif

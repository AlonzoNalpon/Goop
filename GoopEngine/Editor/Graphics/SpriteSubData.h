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
    GLuint    width, height;  //!< integral sprite dimensions in width and height (x and y)
    // It is possible that sprite data can have varying sizes which requires offsets. 
    // This is the place to put offset if it's to be implemented (in pixels based on dimensions).
    // pivot offset should be done in model space first before game object transform
  };
}


#endif

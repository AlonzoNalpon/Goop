#ifndef CHARACTER_H
#define CHARACTER_H
#include <Graphics/Def/GraphicsTypes.h>
#include <def.h>
namespace Graphics::Fonts
{
  struct Character 
  {
    GLuint textureID; //!< texture handle of glyph tex
    gIvec2 size;      //!< glyph size
    gIvec2 bearing;   //!< offset from baseline to left/top of glyph
    GLuint advance;   //!< offset to advance to next glyph (for rendering a string)

    Character() :
      textureID{ }, size{ }, bearing{ }, advance{ } {}
    Character(GLuint textureID, gIvec2 size, gIvec2 bearing, GLuint advance) :
      textureID{ textureID }, size{ size }, bearing{ bearing }, advance{ advance } {}
  };
}

#endif

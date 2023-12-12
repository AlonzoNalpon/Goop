/*!*********************************************************************
\file   Character.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains definition of character struct for fonts
        This is to be used with freetype's library
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef CHARACTER_H
#define CHARACTER_H
#include <Graphics/Def/GraphicsTypes.h>
#include <def.h>
namespace GE::Graphics::Fonts
{
  /*!
   * \struct Character
   * \brief  
   * struct to store character information from a font.
   */
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

/*!*********************************************************************
\file   SpriteData.h
\author a.nalpon\@digipen.edu
\date   16-September-2023
\brief  This file contains definition to the class, SpriteData.
  Sprite data differs from texture as more than 1 SpriteData could
  point to the same Texture object (to support texture atlas in future).
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H
#include <Graphics/SpriteSubData.h>
#include <Graphics/Texture.h>
namespace GE::Graphics {
  /*!
   * \struct SpriteData
   * \brief  
   * Contains data specific to sprites, including dimensions, 
   * texture coordinates, and a texture handle..
   */
  struct SpriteData {
    SpriteSubData info;           //!< texture info including texcoords and dimensions
    GLuint        texture;        //!< texture object handle (contains the texture to refer to)
    SpriteData(SpriteSubData spriteInfo, GLuint texID) : info{ spriteInfo }, texture{ texID } {}
    SpriteData() : info{}, texture{} {}
    void SetTint(Colorf clr) { info.SetTint(clr); }
  };
}

#endif

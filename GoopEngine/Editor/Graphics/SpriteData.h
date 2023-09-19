/*!*********************************************************************
\file   SpriteData.h
\author a.nalpon@digipen.edu
\date   16-September-2023
\brief  This file contains definition to the class, SpriteData.
  Sprite data differs from texture as more than 1 SpriteData could
  point to the same Texture object (to support texture atlas in future).
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Texture.h>
namespace Graphics {
  /*!
   * \struct SpriteData
   * \brief  
   * Contains data specific to sprites, including dimensions, 
   * texture coordinates, and a texture handle..
   */
  struct SpriteData {
    Texture texture;        //!< texture object handle (contains the texture to refer to)
    Vec2    texCoords;      //!< texture coordinates of sprite (important for tex atlas support)
    GLuint  width, height;  //!< integral sprite dimensions in width and height (x and y)
    // It is possible that sprite data can have varying sizes which requires offsets. 
    // This is the place to put offset (in pixels based on dimensions).
    // pivot offset should be done in model space first before game object transform
  };
}

#endif
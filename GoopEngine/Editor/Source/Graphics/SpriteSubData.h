/*!*********************************************************************
\file   SpriteSubData.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition to SpriteSubData struct
        for sprite information (as well as sprite animation frame
        information)
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_SUB_DATA
#define SPRITE_SUB_DATA

#include <Graphics/Def/GraphicsTypes.h>
namespace GE::Graphics {
  /*!
   * \struct SpriteData
   * \brief
   * Contains sprite texcoords and dimensions but NOT texture ID (to be used by sprite animator and spriteData struct)
   */
  struct SpriteSubData {
    gVec2     texCoords;      //!< texture coordinates of sprite (important for tex atlas support)
    gVec2     texDims;        //!< texture dimensions of a sprite in float ranging from [0,1] (for shader uniforms)
    GLuint    width, height;  //!< integral sprite pixel dimensions in width and height (x and y) (can be useful in the future)
    Colorf    tint;           //!< tint of sprite
    Colorf    multiply;       //!< multiplied color (after tint)

    SpriteSubData(gVec2 _texCoords, gVec2 _texDims, GLuint _width, GLuint _height, 
      Colorf _tint = Colorf(1.f, 1.f, 1.f, 0.f), Colorf _multiply = Colorf(1.f,1.f,1.f,1.f)) : 
      texCoords{ _texCoords }, texDims{ _texDims }, width{ _width }, height{ _height }, tint{ _tint }, multiply{_multiply} {}

    SpriteSubData() : texCoords{}, texDims{ 1, 1 }, width{}, height{}, 
      tint{ 1.f, 1.f, 1.f, 0.f }, multiply{1.f,1.f,1.f,1.f} {}
    void SetTint(Colorf clr) { tint = clr; }

    void SetMult(Colorf clr) { multiply = clr; }

    void SetTint(GLfloat r, GLfloat g, GLfloat b, GLfloat a) { SetTint({r,g,b,a}); }

    inline SpriteSubData& operator=(SpriteSubData const& rhs)
    {
      texCoords = rhs.texCoords;
      texDims = rhs.texDims;
      width = rhs.width;
      height = rhs.height;
      tint = rhs.tint;
      multiply = rhs.multiply; 
      return *this;
    }
    inline void CopyDataNoColor(SpriteSubData const& rhs)
    {
      texCoords = rhs.texCoords;
      texDims = rhs.texDims;
      width = rhs.width;
      height = rhs.height;
    }
    // It is possible that sprite data can have varying sizes which requires offsets. 
    // This is the place to put offset if it's to be implemented (in pixels based on dimensions).
    // pivot offset should be done in model space first before game object transform
  };
}


#endif

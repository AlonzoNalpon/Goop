/*!*********************************************************************
\file   SpriteAnimGenerator.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition for the SpriteAnimGenerator
        class. It is a stateless namespace that holds static functions
        only.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIM_GENERATOR_H
#define SPRITE_ANIM_GENERATOR_H
#include <SpriteAnimation/SpriteAnimGenerator.h>
#include <SpriteAnimation/SpriteAnimation.h>
#include <SpriteAnimation/Def/SpriteAnimGenFormat.h>
namespace GE::Graphics{
  /*!
   * \class SpriteAnimGenerator
   * \brief  
   * Stateless class that holds static methods for generating sprites.
   */
  class SpriteAnimGenerator 
  {
  public:
    /*!*********************************************************************
    \brief
      Generate an animation dataset based on parameters.
    \params
      slices  the number of slices in texture
      stacks  the number of stacks in texture
      frames  the number of frames in texture
      spriteW the width of texture
      spriteH the height of texture
      speed   the speed of the animation, measured in time per frame
      flags   flags of the animation
      texture the texture ID
    \return the sprite animation data
      
    ************************************************************************/
    static SpriteAnimation GenerateAnimData(GLuint slices, GLuint stacks, GLuint frames, 
      GLuint spriteW, GLuint spriteH, f64 speed = 1, u32 flags = 0u, GLuint texture = 0);
  private:
  };

}

#endif
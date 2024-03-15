/*!*********************************************************************
\file   SpriteAnimation.h
\author a.nalpon\@digipen.edu
\date   16-September-2023
\brief  This file contains the definition to SpriteAnimation class
to store sprite animation data
  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/SpriteData.h>
#include <SpriteAnimation/Def/SpriteAnimationFlags.h>
namespace GE::Graphics 
{
  /*!
   * \struct SpriteAnimation
   * \brief  
   * Sprite animation class to store animation data for
   * reference by sprite animation instances.
   * This info does not store texture (should be reading from sprite component)
   */
  struct SpriteAnimation {
    f64                         speed;  //!< speed in frames per second
    std::vector<SpriteSubData>  frames; //!< every frame stored as sprite data
    u32                         flags;   //!< refer to the enumerated flags
    GLuint                      texture; //!< the texture ID
    inline f64 GetTotalAnimTime()const
    {
      return speed * static_cast<double>(frames.size());
    }
  };
}
#endif
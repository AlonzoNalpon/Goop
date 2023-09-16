/*!*********************************************************************
\file   SpriteAnimation.h
\author a.nalpon@digipen.edu
\date   16-September-2023
\brief  This file contains the definition to SpriteAnimation class
to store sprite animation data
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H
#include <GLApp/GraphicsTypes.h>
#include <GLApp/Graphics/SpriteData.h>
#include <GLApp/FlagsDef/SpriteAnimationFlags.h>
namespace Graphics 
{
  /*!
   * \struct SpriteAnimation
   * \brief  
   * Sprite animation class to store animation data for
   * reference by sprite animation instances
   */
  struct SpriteAnimation {
    f64                     speed;  //!< speed in frames per second
    std::vector<SpriteData> frames; //!< every frame stored as sprite data
    u32                     flags;   //!< refer to the enumerated flags
    // There's 4 free bytes of padding here for the taking
  };
}
#endif
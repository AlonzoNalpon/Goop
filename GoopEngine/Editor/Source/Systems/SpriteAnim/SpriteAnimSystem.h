/*!*********************************************************************
\file   SpriteAnimSystem.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition of the SpriteAnimSystem class  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIM_SYSTEM_H
#define SPRITE_ANIM_SYSTEM_H
#include <ECS/System/System.h>
#include <iostream>

// The involved components:
#include <Component/Sprite.h>			// sprite
#include <Component/SpriteAnim.h> // Will edit the sprite component
namespace GE::Systems {
  /*!
   * \class SpriteAnimSystem
   * \brief  
   * System that updates all sprites based on sprite animation instance.
   * \note this class only updates sprite animation instances but 
   * eventually there should be an animation controller to support more
   * behaviours
   */
  class SpriteAnimSystem : public GE::ECS::System
  {
  public:
    virtual void Start();

    virtual void Update();

    static void SetAnimation(ECS::Entity entity, size_t animID);
  };
}
#endif

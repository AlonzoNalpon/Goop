/*!*********************************************************************
\file   SpriteAnimSystem.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition of the SpriteAnimSystem class  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
    /*!*********************************************************************
    \brief
      Initializes the system and retrieves initial data required from the
      entities
    ************************************************************************/
    virtual void Start();

    /*!*********************************************************************
    \brief
      Update function of the system. Iterates through all entities in the
      system and updates their sprite animation if needed.
    ************************************************************************/
    virtual void Update();

    /*!*********************************************************************
    \brief
      Stes the animation of an entity
    \param entity
      The entity
    \param animID
      The ID of the animation
    ************************************************************************/
    static void SetAnimation(ECS::Entity entity, size_t animID);
  };
}
#endif

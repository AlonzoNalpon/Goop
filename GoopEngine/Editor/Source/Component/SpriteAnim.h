/*!*********************************************************************
\file   SpriteAnim.h
\author a.nalpon\@digipen.edu
\date   2-November-2023
\brief  this file contains the definition of sprite animation component
        for ECS

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIM_COMPONENT_H
#define SPRITE_ANIM_COMPONENT_H

namespace GE::Component
{
  struct SpriteAnim
  {
    SpriteAnim() = default;
    SpriteAnim(Graphics::gObjID animID) : animID{ animID } {}

    f64 currTime{};
    Graphics::gObjID animID{}; // object ID
    u32 currFrame{};
    u32 flags{};
  };
}
#endif

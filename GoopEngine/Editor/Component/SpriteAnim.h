#ifndef SPRITE_ANIM_COMPONENT_H
#define SPRITE_ANIM_COMPONENT_H
#include <pch.h>
namespace GE::Component
{
  struct SpriteAnim : public GE::ECS::Component
  {
    f64 currTime;
    gObjID animID;
    u32 currFrame;
  };
}
#endif

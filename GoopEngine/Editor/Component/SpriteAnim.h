#ifndef SPRITE_ANIM_COMPONENT_H
#define SPRITE_ANIM_COMPONENT_H
#include <pch.h>
namespace GE::Component
{
  struct SpriteAnim
  {
    f64 currTime{};
    Graphics::gObjID animID{}; // object ID
    u32 currFrame{};
  };
}
#endif

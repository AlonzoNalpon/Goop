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

#ifndef SPRITE_ANIM_COMPONENT_H
#define SPRITE_ANIM_COMPONENT_H
namespace GE::Component
{
  struct SpriteAnim
  {
    SpriteAnim() = default;
    SpriteAnim(Graphics::gObjID animID) : m_animID{ animID } {}

    f64 m_currTime{};
    Graphics::gObjID m_animID{}; // object ID
    u32 m_currFrame{};
  };
}
#endif

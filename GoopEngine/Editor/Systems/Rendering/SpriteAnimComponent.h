#ifndef SPRITE_ANIM_COMPONENT_H
#define SPRITE_ANIM_COMPONENT_H

#include <Graphics/Def/GraphicsTypes.h>
#include <ECS/Component/Component.h>
namespace Components
{
  struct SpriteAnim : public GE::ECS::Component
  {
    u32 currFrame;  //!< the current frame of the animation
    u32 flags;      //!< flags to signal completion and so on (not implemented)
  };
}

#endif
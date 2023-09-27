#ifndef SPRITE_ANIM_SYSTEM_H
#define SPRITE_ANIM_SYSTEM_H
#include <pch.h>
#include <iostream>

// The involved components:
#include <Component/Sprite.h>			// sprite (ALL MUST HAVE THIS)
#include <Component/SpriteAnim.h> // optional and will edit the sprite component
namespace GE::Systems {
  class SpriteAnimSystem : public GE::ECS::System
  {
  public:

    void Update();

    void OnDestroyed();
  };
}
#endif

#include "SpriteAnimationFlags.h"

namespace Graphics {
  u32 operator|(u32 lhs, SPRITE_ANIM_FLAGS rhs){ return lhs | static_cast<u32>(rhs); }
  u32& operator|=(u32& lhs, SPRITE_ANIM_FLAGS rhs){ return lhs |= static_cast<u32>(rhs); }

}
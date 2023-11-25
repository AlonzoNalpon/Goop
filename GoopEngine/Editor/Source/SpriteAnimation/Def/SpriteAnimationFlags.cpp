/*!*********************************************************************
\file   SpriteAnimationFlags.cpp
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains all definitions for SpriteAnimationFlags
scoped enum methods. Mostly bit twiddling stuff
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <SpriteAnimation/Def/SpriteAnimationFlags.h>

namespace Graphics {
  u32 operator|(u32 lhs, SPRITE_ANIM_FLAGS rhs){ return lhs | static_cast<u32>(rhs); }
  u32& operator|=(u32& lhs, SPRITE_ANIM_FLAGS rhs){ return lhs |= static_cast<u32>(rhs); }
  u32 operator&(u32 lhs, SPRITE_ANIM_FLAGS rhs){return lhs & static_cast<u32>(rhs);}
  u32& operator&=(u32& lhs, SPRITE_ANIM_FLAGS rhs){return lhs &= static_cast<u32>(rhs);}
  u32 operator^(u32 lhs, SPRITE_ANIM_FLAGS rhs){return lhs ^ static_cast<u32>(rhs);}
  u32& operator^=(u32& lhs, SPRITE_ANIM_FLAGS rhs){return lhs ^= static_cast<u32>(rhs);}

  u32 operator~(SPRITE_ANIM_FLAGS val){return ~static_cast<u32>(val);}

}
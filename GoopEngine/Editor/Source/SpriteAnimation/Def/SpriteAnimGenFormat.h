/*!*********************************************************************
\file   SpriteAnimGenFormat.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains definition for sprite animation generator formats.
There's no use for this and it should be removed in the future
unless there will be more ways to read a spritesheet than left to right
row major. (And we hope there won't be any)
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SPRITE_ANIM_GEN_FORMAT_H
#define SPRITE_ANIM_GEN_FORMAT_H
namespace GE::Graphics {

  enum class SPRITE_ANIM_FORMAT
  {
    TORIGHT_TOBOTTOM // the standard
  };
}
#endif

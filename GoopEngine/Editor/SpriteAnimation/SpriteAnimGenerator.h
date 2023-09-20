#ifndef SPRITE_ANIM_GENERATOR_H
#define SPRITE_ANIM_GENERATOR_H
#include <SpriteAnimation/SpriteAnimGenerator.h>
#include <SpriteAnimation/SpriteAnimation.h>
#include <SpriteAnimation/Def/SpriteAnimGenFormat.h>
namespace Graphics{

  class SpriteAnimGenerator 
  {
  public:
    static SpriteAnimation GenerateAnimData(GLint hCuts, GLint vCuts, gObjID texture);
  private:
    static SpriteAnimation AnimDataLtR_TtB();
  };

}

#endif
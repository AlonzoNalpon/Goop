#ifndef SPRITE_ANIM_GENERATOR_H
#define SPRITE_ANIM_GENERATOR_H
#include <SpriteAnimation/SpriteAnimGenerator.h>
#include <SpriteAnimation/SpriteAnimation.h>
#include <SpriteAnimation/Def/SpriteAnimGenFormat.h>
namespace Graphics{

  class SpriteAnimGenerator 
  {
  public:
    static SpriteAnimation GenerateAnimData(GLuint hFrames, GLuint vFrames, GLuint spriteW, GLuint spriteH, f64 speed = 1, u32 flags = 0u, gObjID texture = BAD_OBJ_ID);
  private:
  };

}

#endif
#include <SpriteAnimation/SpriteAnimGenerator.h>
#include <DebugTools/Exception/Exception.h>
namespace Graphics {
  SpriteAnimation SpriteAnimGenerator::GenerateAnimData(GLuint hFrames, GLuint vFrames, GLuint spriteW, GLuint spriteH, f64 speed, u32 flags, gObjID texture)
  {
    if (!hFrames || !vFrames || !spriteW || !spriteH)
    {
      throw GE::Debug::Exception<SpriteAnimGenerator>(GE::Debug::LEVEL_ERROR, ErrMsg("Sprite animation has 0 frames/dimensions!"));
    }
    SpriteAnimation retval{ speed,{},flags,texture };
    retval.frames.reserve(hFrames * vFrames); // reserve the number of frames in this animation
    GLuint const xStep{ spriteW / hFrames }, yStep{ spriteH/vFrames }; // steps to iterate through image position
    GLuint posY{ spriteH - yStep }; // we are starting from the top left frame

    // float dimensions calculated. (same for every frame)
    gVec2 dims{ static_cast<GLfloat>(xStep)/spriteW, static_cast<GLfloat>(yStep)/spriteH};
    for (GLuint y{}; y < vFrames; ++y)
    {
      GLuint  posX{}; // position in pixels (integers)
      GLfloat posYf{ static_cast<GLfloat>(posY) / spriteH }; // y position in texcoords
      for (GLuint x{}; x < hFrames; ++x)
      {
        GLfloat posXf{ static_cast<GLfloat>(posX) / spriteW }; // x position in texcoords
        retval.frames.emplace_back(
              gVec2{ posXf, posYf },  // texcoords
              dims,                   // dimensions in float
              xStep,                  // width
              yStep                   // height
          );

        posX += xStep; // update x pos to new origin of next sprite
      }
      posY -= yStep; // update y pos to new origin of next sprite
    }

    return retval;
  }
}

/*!*********************************************************************
\file   SpriteAnimGenerator.cpp
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation of the SpriteAnimGenerator
class. It's for generating sprite animation information based on
parameters, independent of actual texture data
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <SpriteAnimation/SpriteAnimGenerator.h>
#include <DebugTools/Exception/Exception.h>
namespace GE::Graphics {
  SpriteAnimation SpriteAnimGenerator::GenerateAnimData(GLuint slices, GLuint stacks, GLuint frames,
    GLuint spriteW, GLuint spriteH, f64 speed, u32 flags, GLuint texture)
  {
    if (!slices || !stacks || !spriteW || !spriteH)
    {
      throw GE::Debug::Exception<SpriteAnimGenerator>(GE::Debug::LEVEL_ERROR, ErrMsg("Sprite animation has 0 frames/dimensions!"));
    }
    SpriteAnimation retval{ speed,{},flags,texture };
    retval.frames.reserve(slices * stacks); // reserve the number of frames in this animation
    GLuint const xStep{ spriteW / slices }, yStep{ spriteH/stacks }; // steps to iterate through image position
    GLuint posY{ spriteH - yStep }; // we are starting from the top left frame

    // float dimensions calculated. (same for every frame)
    gVec2 dims{ static_cast<GLfloat>(xStep)/spriteW, static_cast<GLfloat>(yStep)/spriteH};
    for (GLuint y{}; y < stacks; ++y)
    {
      GLuint  posX{}; // position in pixels (integers)
      GLfloat posYf{ static_cast<GLfloat>(posY) / spriteH }; // y position in texcoords
      for (GLuint x{}; x < slices; ++x)
      {
        GLfloat posXf{ static_cast<GLfloat>(posX) / spriteW }; // x position in texcoords
        retval.frames.emplace_back(
              gVec2{ posXf, posYf },  // texcoords
              dims,                   // dimensions in float
              xStep,                  // width
              yStep                   // height
          );
        if (!--frames) 
        {
          return retval; // stop if we hit the number of frames
        }
        posX += xStep; // update x pos to new origin of next sprite
      }
      posY -= yStep; // update y pos to new origin of next sprite
    }

    return retval;
  }
}

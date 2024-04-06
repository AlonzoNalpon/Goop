/*!*********************************************************************
\file   FrameBufferInfo.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition of a struct to hold
        information related to a particular framebuffer.
  
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef FRAME_BUFFER_INFO_H
#define FRAME_BUFFER_INFO_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Renderer/Camera.h>
namespace GE::Graphics::Rendering
{

  struct FrameBufferInfo
  {
    gObjID objID{};             //!< the object's ID in graphics engine
    GLuint frameBuffer{};       //!< framebuffer handle
    GLuint renderTexture{};     //!< the texture that has the framebuffer
    Rendering::Camera camera;   //!< camera information
    gIvec2 vpDims{};
  };
}
#endif

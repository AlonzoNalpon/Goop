/*!*********************************************************************
\file   Texture.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition of the texture class
  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H
#include <Def.h>
/*!
 * \brief  
 * This class contains a texture handle to a texture object, along with other attributes
 * The class is also in charge of allocation/deallocation of texture objects
 * stored in the GPU.
 */
namespace GE::Graphics {

  struct Texture
  {
    GLuint textureHandle;     //!< the handle to the texture
    GLenum filtering;         //!< the filtering mode for the image !NOT USED FOR NOW
    GLenum format;            //!< format of data (RGBA) !NOT USED FOR NOW
    GLenum internalFormat;    //!< layout of pixel data in GPU(RGBA8) !NOT USED FOR NOW
    GLuint width, height;     //!< dimensions of the image
  };
}

#endif
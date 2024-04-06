/*!*********************************************************************
\file   Model.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition for the model structure
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef MODEL_H
#define MODEL_H
#include <Def.h>
/*!
 * \struct Model
 * \brief  
 * This struct contains basic information required for rendering objects.
 * 
 */
namespace GE::Graphics
{
  struct Model {
    GLenum primitive_type; //!<which OpenGL primitive to be rendered? (triangle strip for example)
    GLuint primitive_cnt;  //!<number of primitives 
    GLuint vaoid;          //!<handle to VAO
    GLuint draw_cnt;       //!<vertex OR indices count
    GLuint shader;         //!<shader handle
  };
}
#endif

/*!*********************************************************************
\file   ShaderProgram.h
\author a.nalpon\@digipen.edu
\date   16-September-2023
\brief  This file contains the definition of the ShaderProgram class.
\note   There's no ShaderProgramManager class as every rendered object
  requires a shader. Therefore they would have a handle to a 
  shaderprogram instead.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include <gl/glew.h>
#include <string>
#include <vector>
namespace Graphics {
  using ShaderInitCont = std::vector<std::pair<GLenum, std::string>>;
  
  /*!
   * \class ShaderProgram
   * \brief  
   * A shader program that contains a handle and its linking status.
   * It will not automatically delete the shader when destroyed. You must
   * manually call DeleteShdrPgm()
   */
  class ShaderProgram {
  public:
    ShaderProgram() : m_pgmHandle{}, m_isLinked{ GL_FALSE } {}


  public: // All foundational methods to create and delete shader program

    /*!*********************************************************************
    \brief
      Compiles shader from a string.
    \params
      shaderType  enumerated type of the shader
      shaderSrc   the string containing the shader's source code
    \return the status of compilation
      
    ************************************************************************/
    GLboolean CompileFromString(GLenum shaderType, std::string const& shaderSrc);

    /*!*********************************************************************
    \brief
      Compiles the shader from a file.
    \params
      shaderType  enumerated type of the shader
      filePath    the name of the shader file. (Automatically determines 
      shader folder path)
    \return status of compilation
      
    ************************************************************************/
    GLboolean CompileFromFile(GLenum shaderType, std::string const& filePath);

    /*!*********************************************************************
    \brief
      Links shader programs.
    \params
    \return status of link
      
    ************************************************************************/
    GLboolean Link();

    /*!*********************************************************************
    \brief
      Check if program was linked.
    \params
    \return status of program
      
    ************************************************************************/
    GLboolean Validate();

    /*!*********************************************************************
    \brief
      Compile, link and validate shader program.
    \params
      
    \return status of shaderprogram
      
    ************************************************************************/
    GLboolean CompileLinkValidate(ShaderInitCont const&);

    /*!*********************************************************************
    \brief
      Get the OpenGL handle of shader program.
    \params
    \return shaderpgm id
      
    ************************************************************************/
    GLuint    GetHandle() const;

    /*!*********************************************************************
    \brief
      Frees the shaderpgm from opengl.
    \params
    \return
    ************************************************************************/
    void      DeleteShdrPgm();
  private:
    GLuint    m_pgmHandle;    //!< shader program handle
    GLboolean m_isLinked;     //!< flag for program linked
  };
}

#endif
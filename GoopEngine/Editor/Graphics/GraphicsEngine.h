/*!*********************************************************************
\file   GraphicsEngine.h
\author a.nalpon@digipen.edu
\date   13-September-2023
\brief  The graphics engine is in charge of rendering including
shader and mesh instances. It aims to abstract away all OpenGL calls.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Def/RenderingHelpers.h>
#include <Graphics/ShaderProgram.h>
#include <map>
namespace Graphics {
  // The graphics engine responsible for any opengl calls
  class GraphicsEngine {
  public:
    GraphicsEngine();
    ~GraphicsEngine();
    /*!*********************************************************************
    \brief
      Initializes the opengl context
    \params
      clearColor 
    \return
    ************************************************************************/
    void Init(Colorf clearColor);

    void Draw();

    /*!*********************************************************************
    \brief attempts to get handle to specified shader program
    \params
      pgmName the name of the shader program
    \return 
      the handle to the program. 0 indicated the program does not exist
      
    ************************************************************************/
    GLuint GetShaderPgm(std::string const& pgmName);

    /*!*********************************************************************
    \brief
      Creates a shader from a vector of shader information.
      If a program of the same name is encountered, it will abort
    \params
      container
        the container filled with associated shader type and shader paths
      name
        the name of the shader program to be created
    \return
      the shader program ID. 0 on failure.
    ************************************************************************/
    GLuint CreateShader(ShaderCont const& container, std::string const& name);


  protected:

    void DrawMdl(Model const&);
    Model GenerateQuad();

    // SHADERS ARE ONLY TO BE QUERIED BY MODELS REQUESTING A HANDLE
    // USERS MUST SPECIFY SHADER NAME WHILE CREATING A MODEL
    std::map<std::string, GLuint> m_shaderLT; //!< LOOKUP TABLE: handles by strings
    std::map<GLuint, ShaderProgram> m_shaders; //!< shaders by ID

    std::map<std::string, GLuint> m_modelsLT; //!< LOOKUP TABLE: handles models by string
    std::vector<Model> m_models; //!< models in a vector (models got their own shader)

    // Textures are separated from models and are to be used with rendering components


    Model m_spriteQuad{}; //!< basic primitive quad for sprites

    // FOR DEBUGGING
  private:
  };
}
#endif
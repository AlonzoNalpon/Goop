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
#include <GLApp/GraphicsTypes.h>
#include <GLApp/Graphics/ShaderProgram.h>
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
    void Init(Color clearColor);
    Model GenerateQuad();
    void Draw(Model const&);
    GLuint GetShader(std::string const&  fileName);
  protected:
    std::map<std::string const, GLuint> m_shaderLT; //!< LOOKUP TABLE: handles by strings
    std::map<GLuint, ShaderProgram> m_shaders; //!< shaders by ID
    Model m_spriteQuad{}; //!< basic primitive quad for sprites

  private:
  };
}
#endif
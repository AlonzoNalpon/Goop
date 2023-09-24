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
#include <Singleton/Singleton.h>
#include <Math/GEM.h>
namespace Graphics {
    
  // The graphics engine responsible for any opengl calls
  class GraphicsEngine : public GE::Singleton<GraphicsEngine>{
    using ShaderLT = std::map<std::string const, gObjID>;
    using ShaderCont = std::vector<ShaderProgram>;
  public:
    GraphicsEngine();
    ~GraphicsEngine();
    /*!*********************************************************************
    \brief initializes opengl viewport, primitives and clear color
      .
    \params
      clearColor the color to clear buffer with
      w width of viewport
      h height of viewport
    \return
    ************************************************************************/
    void Init(Colorf clearColor, GLint w, GLint h);

    void Draw();

    /*!*********************************************************************
    \brief attempts to get handle to specified shader program
    \params
      pgmName the name of the shader program
    \return 
      the handle to the program. 0 indicated the program does not exist
      
    ************************************************************************/
    gObjID GetShaderPgm(std::string const& pgmName);

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
    GLuint CreateShader(ShaderInitCont const& container, std::string const& name);

    /*!*********************************************************************
    \brief Get a handle to the renderer
      .
    \params
    \return the renderer held by graphics engine
      
    ************************************************************************/
    Rendering::Renderer& GetRenderer();

    gObjID GetModel();
  public: // DRAW PRIMITIVE METHODS
    /*!*********************************************************************
    \brief Draws a line in world coordinates (0,0 is center of screen)
      .
    \params
      startPt start point in world space
      endPt end point in world space
      clr the color of the line
    \return
    ************************************************************************/
    static void DrawLine(GE::Math::dVec2 const& startPt, GE::Math::dVec2 const& endPt, Colorf clr = {1, 0, 0});

  protected:
    GLint m_vpWidth, m_vpHeight; //!< dimensions of viewport
    GLfloat m_ar; //!< aspect ratio
    void DrawMdl(Model const& mdl);
    void DrawMdl(Model const& mdl, SpriteData const& sprite);
    void DrawMdl(Model const& mdl, SpriteData const& sprite, SpriteAnimation anim, GLuint frame);
    Model GenerateQuad();
    Model GenerateLine();
    // SHADERS ARE ONLY TO BE QUERIED BY MODELS REQUESTING A HANDLE
    // USERS MUST SPECIFY SHADER NAME WHILE CREATING A MODEL
    ShaderLT                        m_shaderLT;         //!< LOOKUP TABLE: handles by strings
    ShaderCont                      m_shaders;          //!< shaders by ID

    std::map<std::string, GLuint>   m_modelsLT;         //!< LOOKUP TABLE: handles models by string
    std::vector<Model>              m_models;           //!< models in a vector (models got their own shader)

    SpriteAnimationManager          m_animManager;      //!< sprite animation manager
    TextureManager                  m_textureManager;   //!< texture manager
    Rendering::Renderer             m_renderer;         //!< renderer in charge of all opengl draw calls
    // Textures are separated from models and are to be used with rendering components


    Model                           m_spriteQuadMdl{};  //!< basic primitive quad for sprites
    Model                           m_lineMdl{};        //!< basic primitive line
    // FOR DEBUGGING
  private:
  public: // getters
    SpriteAnimationManager const& animManager{ m_animManager };
    TextureManager const&         textureManager{ m_textureManager };
  };
}
#endif
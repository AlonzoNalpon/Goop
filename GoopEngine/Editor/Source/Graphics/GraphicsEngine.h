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
#include <Graphics/Fonts/FontManager.h>
namespace Graphics {
    
  // The graphics engine responsible for any opengl calls
  class GraphicsEngine : public GE::Singleton<GraphicsEngine>
  {
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

    void ClearBuffer();

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

    /*!*********************************************************************
    \brief
      Initializes a texture from data.
    \params
      name name of the texture to be stored
      imageData image data
    \return
      
    ************************************************************************/
    gObjID InitTexture(std::string const& name, GE::Assets::ImageData const& imageData);

    /*!*********************************************************************
    \brief
      Creates an animation dataset. Will assume frames from left to right, top to bottom.
      ROW MAJOR
    \params
      name      the name of the animation
      slices    slices in the sprite
      stacks    stacks in the sprite
      frames    number of frames in the sprite
      speed     speed of the animation
      flags     flags of the animation
      textureID texture ID of the animation (which texture is this based on?)
    \return
      
    ************************************************************************/
    gObjID CreateAnimation(std::string const& name, GLuint slices, GLuint stacks, GLuint frames,
      f64 speed, u32 flags, gObjID textureID);

    /*!*********************************************************************
    \brief
      Gets basic model for all sprites.
    \note if more models are made for gameobjects, this has to be changed
    \params
    \return the quad model for sprites
      
    ************************************************************************/
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

    /*!*********************************************************************
    \brief
      Generates a quad with pos, color and texture coordinate attributes
    \params
    \return quad model
      
    ************************************************************************/
    Model GenerateQuad();

    /*!*********************************************************************
    \brief
      Generates a line model. This is for debug draws
    \params
    \return line model
      
    ************************************************************************/
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
    Fonts::FontManager              m_fontManager;      //!< font manager to store font data

    // Textures are separated from models and are to be used with rendering components

    Model                           m_spriteQuadMdl{};  //!< basic primitive quad for sprites
    Model                           m_lineMdl{};        //!< basic primitive line
    // FOR DEBUGGING
  private:
  public: // getters
    SpriteAnimationManager const&   animManager{ m_animManager };      // read-only getter to animation manager 
    TextureManager const&           textureManager{ m_textureManager };// read-only getter to texture manager
  };
}
#endif
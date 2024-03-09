/*!*********************************************************************
\file   GraphicsEngine.h
\author a.nalpon\@digipen.edu
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
#include <Graphics/Renderer/FrameBufferInfo.h>
namespace GE::Graphics {

  // The graphics engine responsible for any opengl calls
  class GraphicsEngine : public GE::Singleton<GraphicsEngine>
  {
    using ShaderLT        = std::map<std::string const, gObjID>;
    using ShaderCont      = std::vector<ShaderProgram>;
    using FB_InfoCont     = std::vector<std::pair<GLuint, Rendering::Camera>>;
    using FB_LT           = std::map<std::string, std::vector<GLuint>::size_type>;
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

    /*!*********************************************************************
    \brief
      Sets the new dimensions to compute the input with

    \param width
      New input region width

    \param height
      New input region height

    \return
    ************************************************************************/
    void SetDim(int width, int height);

    /*!*********************************************************************
    \brief Initializes the main framebuffer.
      DEPRECATED: This function is to be replaced in future revisions.
    \params
    \return
    ************************************************************************/
    void InitFrameBuffer();

    /*!*********************************************************************
    \brief
      Gets main render texture of framebuffer.
      DEPRECATED: This function is to be replaced in future revisions.
    \params
    \return
      
    ************************************************************************/
    GLuint GetRenderTexture();

    /*!*********************************************************************
    \brief
      Clear the buffer of whichever attached framebuffer.
    \params
    \return
    ************************************************************************/
    void ClearBuffer();

    /*!*********************************************************************
    \brief
      Draws all objects onto framebuffers. Does not include rendering to
      screen
    \params
    \return
    ************************************************************************/
    void Draw();

    /*!*********************************************************************
    \brief
      Renders framebuffer onto screen using a quad.
    \params
      framebufferID ID of buffer to render with
    \return
    ************************************************************************/
    void RenderToScreen(gObjID framebufferID = 0);

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
    \return the ID for the texture which can also be used directly with opengl
      
    ************************************************************************/
    GLuint InitTexture(std::string const& name, GE::Assets::ImageData const& imageData);

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
      f64 speed, u32 flags, GLuint textureID);

    /*!*********************************************************************
    \brief
      Gets basic model for all sprites.
    \note if more models are made for gameobjects, this has to be changed
    \params
    \return the quad model for sprites
      
    ************************************************************************/
    gObjID GetModel();

    /*!*********************************************************************
    \brief
      Obtain the viewport width, in pixels.
    \params
    \return
      
    ************************************************************************/
    GLint GetVPWidth();

    /*!*********************************************************************
    \brief
      Obtain the viewport height, in pixels.
    \params
    \return
      
    ************************************************************************/
    GLint GetVPHeight();

    /*!*********************************************************************
    \brief
      Obtain the position of mouse in worldspace.
    \params
      mousePos  mouse position in single precision float
      frameBuffer ID of the framebuffer
    \return
      
    ************************************************************************/
    gVec2 ScreenToWS(gVec2 const& mousePos, gObjID frameBuffer);

    /*!*********************************************************************
    \brief
      Create a framebuffer with specified with and height.
    \params
      width
      height
    \return ID of created framebuffer
      
    ************************************************************************/
    gObjID CreateFrameBuffer(GLint width, GLint height);

    /*!*********************************************************************
    \brief
      Get framebuffer information from associated ID.
    \params
      id  ID of framebuffer
    \return
      
    ************************************************************************/
    Rendering::FrameBufferInfo& GetFrameBuffer(gObjID id);

    /*!*********************************************************************
    \brief
      Gets animation mutable manager. use animManager instead if not editing
    \params
    \return
      
    ************************************************************************/
    SpriteAnimationManager& GetAnimManager();
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
    void DrawLine(GE::Math::dVec2 const& startPt, GE::Math::dVec2 const& endPt, Colorf clr = {1, 0, 0});

    /*!*********************************************************************
    \brief
      Destroys a texture using texture manager.
    \params
      texture
    \return
    ************************************************************************/
    void DestroyTexture(GLuint texture);

    /*!*********************************************************************
    \brief
      Free textures and fonts in the gpu
    \return
    ************************************************************************/
    void FreeTexturesAndFonts();
  protected:
    GLint m_vpWidth, m_vpHeight; //!< dimensions of viewport
    GLfloat m_ar; //!< aspect ratio



    /*!*********************************************************************
    \brief
      Generates a quad with pos, color and texture coordinate attributes
    \params
    \return quad model
      
    ************************************************************************/
    Model GenerateQuad(GLfloat width = 0.5f, GLfloat height = 0.5f);

    /*!*********************************************************************
    \brief
      Generates a line model. This is for debug draws
    \params
    \return line model
      
    ************************************************************************/
    Model GenerateLine();

    /*!*********************************************************************
    \brief
      Generates font model.
    \params
    \return

    ************************************************************************/
    Model GenerateFontMdl();


    // SHADERS ARE ONLY TO BE QUERIED BY MODELS REQUESTING A HANDLE
    // USERS MUST SPECIFY SHADER NAME WHILE CREATING A MODEL
    
    std::map<gObjID,
      Rendering::FrameBufferInfo>     m_frameBuffers;     //!< Every framebuffer is stored in here
    GLuint                            m_framebuffer;
    GLuint                            m_renderTexture;

    ShaderLT                          m_shaderLT;         //!< LOOKUP TABLE: handles by strings
    ShaderCont                        m_shaders;          //!< shaders by ID

    std::map<std::string, GLuint>     m_modelsLT;         //!< LOOKUP TABLE: handles models by string
    std::vector<Model>                m_models;           //!< models in a vector (models got their own shader)

    SpriteAnimationManager            m_animManager;      //!< sprite animation manager
    TextureManager                    m_textureManager;   //!< texture manager
    Rendering::Renderer               m_renderer;         //!< renderer in charge of all opengl draw calls
    Fonts::FontManager                m_fontManager;      //!< font manager to store font data

    // Textures are separated from models and are to be used with rendering components

    Model                           m_spriteQuadMdl{};  //!< basic primitive quad for sprites
    Model                           m_renderQuad{};     //!< rendering quad to cover screen
    Model                           m_lineMdl{};        //!< basic primitive line
    Model                           m_fontMdl{};        //!< font model quad for rendering text

    // FOR DEBUGGING
  private:
  public: // getters
    SpriteAnimationManager const&   animManager{ m_animManager };      // read-only getter to animation manager 
    TextureManager const&           textureManager{ m_textureManager };// read-only getter to texture manager
    Fonts::FontManager &       fontManager{ m_fontManager };
  };
}
#endif
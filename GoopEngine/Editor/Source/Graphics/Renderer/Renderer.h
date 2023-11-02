/*!*********************************************************************
\file   Renderer.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition for Renderer class.
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef RENDERER_H
#define RENDERER_H
#include <Graphics/Def/GraphicsTypes.h>
#include <Graphics/Def/RenderingHelpers.h>
#include <Graphics/Renderer/RenderData.h>
#include <Graphics/TextureManager.h>
#include <Graphics/Renderer/Camera.h>
#include <Graphics/Fonts/FontManager.h>
#include <Graphics/Renderer/FrameBufferInfo.h>
namespace Graphics::Rendering
{
  /*!
   * \class Renderer
   * \brief  
   * class to perform rendering for graphics engine.
   * \note In the future, render order will be a problem, especially with transparency
   * This class is meant to solve it with future implementations.
   * 
   * reduced OGL call optimizations should be made in this implementation
   */
  class Renderer {
    using ShaderLT = std::map<std::string const, gObjID>;
    using ShaderCont = std::vector<ShaderProgram>;
  public:

    
    Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager,
      ShaderCont const& shaderCont, Fonts::FontManager const& fontManager, GLint const& vpWidth, GLint const& vpHeight,
      std::map < gObjID, FrameBufferInfo> const& frameBuffers);

    void ClearRenderContainers();
    // OLD CHANGE
    // Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager, ShaderCont const& shaderCont
    // , Fonts::FontManager const& fontManager, GLint const& vpWidth, GLint const& vpHeight);

    /*!*********************************************************************
    \brief
      Initializes the renderer with a camera object and size for 
      render call buffer.
    \params
      camera the camera information for the scene
      renderCallSize the size of the container for render calls
    \return
    ************************************************************************/
    void Init(Camera const& camera, gObjID lineMdlID, size_t renderCallSize = 2048);

    /*!*********************************************************************
    \brief
      Queues an order to render object.
    \params
      mdl       the model of the object
      sprite    the sprite of the object
      transform the transform of the object
    \return
    ************************************************************************/
    void RenderObject(gObjID mdl, SpriteData const& sprite, GE::Math::dMat4 const& trans);
    
    void RenderFontObject(gVec2 pos, GLfloat scale, std::string const& str, Colorf color, gObjID fontID);

    void RenderLineDebug(GE::Math::dVec2 const& startPt, GE::Math::dVec2 const& endPt, Colorf const& clr);

    /*!*********************************************************************
    \brief
      Draws all object in render data container. Clears the container after.
    \params
    \return
    ************************************************************************/
    void Draw(Camera & camera);

    /*!*********************************************************************
    \brief
      Clears all render data.
    \params
    \return
    ************************************************************************/
    void ClearRenderData();

    /*!*********************************************************************
    \brief
      Draws font object from string and other specs.
      Does not deal with binding using shaders.
      This function will be hidden in future revisions.
      ALPHA NOT YET SUPPORTED
    \params
      str string of text
      pos position of text
      scale scale of text
      clr color of text
      fontID ID of font to use
      camera Camera to transform text
    \return
    ************************************************************************/
    void DrawFontObj(std::string const& str, gVec2 pos, GLfloat scale, Colorf const& clr, Graphics::gObjID fontID, Camera& camera);

    /*!*********************************************************************
    \brief
      Gets reference to camera object.
    \params
    \return
      
    ************************************************************************/
    Camera& GetCamera();
  private:
    // Private methods
    /*!*********************************************************************
    \brief
      Calculates the transform based on scale, rotation and position.
    \params
      scale
      rotation
      pos
    \return the matrix result
      
    ************************************************************************/
    glm::mat4 CalculateTransform(gVec3 const& scale, GLfloat rotation, gVec3 const& pos) const;

  private:
    std::vector<RenderData>             m_renderCalls;     //!< container for all render calls
    std::vector<FontRenderData>         m_fontRenderCalls; //!< font
    std::vector<LineRenderData>         m_lineRenderCalls; //!< DEBUG drawing lines with colors

    Camera                              m_camera;   //!< camera for rendering

    std::vector<Model> const&           r_mdlContainer; //!< reference to the model container
    TextureManager const&               r_texManager;   //!< reference to the texture manager
    ShaderCont const&                   r_shaders;      //!< reference to the shader container
    Fonts::FontManager const&           r_fontManager;  //!< reference to the font manager
    GLint const&                        r_vpWidth;
    GLint const&                        r_vpHeight;
    std::map < gObjID, 
      FrameBufferInfo> const&           r_frameBuffers; //!< reference to framebuffers
    // primitive models
    gObjID                              m_lineMdlObj;   //!< handle to line model
  };
}
#endif

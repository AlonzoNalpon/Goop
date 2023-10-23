/*!*********************************************************************
\file   Renderer.h
\author a.nalpon@digipen.edu
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

    
    Renderer(std::vector<Model> const& mdlContainer, TextureManager const& texManager, ShaderCont const& shaderCont
    , Fonts::FontManager const& fontManager, DeferredInfo const& deferredInfo, Model const& vpQuadMdl);

    /*!*********************************************************************
    \brief
      Initializes the renderer with a camera object and size for 
      render call buffer.
    \params
      camera the camera information for the scene
      renderCallSize the size of the container for render calls
    \return
    ************************************************************************/
    void Init(Camera const& camera, size_t renderCallSize = 2048);

    /*!*********************************************************************
    \brief
      Queues an order to render object.
    \params
      mdl       the model of the object
      sprite    the sprite of the object
      transform the transform of the object
    \return
    ************************************************************************/
    void RenderObject(gObjID mdl, SpriteData const& sprite, Transform const& transform);
    
    /*!*********************************************************************
    \brief
      Draws all object in render data container. Clears the container after.
    \params
    \return
    ************************************************************************/
    void Draw();
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
    /*!*********************************************************************
    \brief
      Calculates the transform based on Transform struct.
    \params
      xForm the transformation data
    \return the matrix result
      
    ************************************************************************/
    glm::mat4 CalculateTransform(Transform const& xForm) const;
  private:
    std::vector<RenderData> m_renderCalls;

    Camera                              m_camera;   //!< camera for rendering

    std::vector<Model> const&           r_mdlContainer; //!< reference to the model container
    TextureManager const&               r_texManager;   //!< reference to the texture manager
    ShaderCont const&                   r_shaders;      //!< reference to the shader container
    Fonts::FontManager const&           r_fontManager;  //!< reference to the font manager

    //GLuint const&                       r_gbuffer;
    //GLuint const&                       r_framebuffer;
    DeferredInfo const&                 r_deferredInfo;
    Model const&                        r_vpQuadMdl;    //!< reference to quad with deferred shader for passing onto final framebuffer
  };
}
#endif
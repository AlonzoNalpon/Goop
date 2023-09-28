/*!*********************************************************************
\file   Camera.h
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the definition for the camera class
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H
#include <Graphics/Def/GraphicsTypes.h>
namespace Graphics::Rendering
{
  /*!
   * \class Camera
   * \brief  
   * This camera class is used for transforming objects in rendering.
   * It currently supports only orthographic
   */
  class Camera
  {
  private:
    glm::mat4 m_view;   //!< view matrix of camera
    glm::mat4 m_proj;  //!< perspective matrix of camera

  public:
    Camera() = default;
    Camera& operator=(Camera const& rhs);

    /*!*********************************************************************
    \brief
      Creates an orthographic camera.
    \params
      pos    position of the camera
      tgt    target of the camera
      up     the up vector of the camera
      left   the left boundary of the camera
      right  the right boundary of the camera
      bottom the bottom boundary of the camera
      top    the top boundary of the camera
      
      
    \return
    ************************************************************************/
    Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 up,
      GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
      GLfloat near, GLfloat far);

    /*!*********************************************************************
    \brief
      Updates the view matrix based on parameters.
    \params
      pos   new position of camera
      tgt   new target of camera
      up    new up vector of camera
    \return
    ************************************************************************/
    void UpdateViewMtx(glm::vec3 pos, glm::vec3 tgt, glm::vec3 up);

    /*!*********************************************************************
    \brief
      Calculates the view projection matrix.
      Avoid calling this multiple times as result is not stored in object
    \params
    \return
      
    ************************************************************************/
    glm::mat4 ViewProjMtx() const;
  public: // getters
    glm::mat4 const& view{ m_view }; //!< getter to view matrix
    glm::mat4 const& proj{ m_proj }; //!< getter to projection matrix
  };

}
#endif

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
    // NOTE: WHEN ADDING VARIABLES, UPDATE ASSIGNMENT OPERATOR
    // (GLM DELETED THEIRS)
    glm::mat4 m_view;     //!< view matrix of camera
    glm::mat4 m_proj;     //!< perspective matrix of camera
    gVec3     m_position; //!< position of camera
    gVec3     m_tgt;
    gVec3     m_up;
    GLfloat   m_left;
    GLfloat   m_right;
    GLfloat   m_bottom;
    GLfloat   m_top;
    GLfloat   m_near;
    GLfloat   m_far;
    GLfloat   m_ar; //!< aspect ratio
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
    Camera(gVec3 pos, gVec3 tgt, gVec3 up,
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
    void UpdateViewMtx(gVec3 pos, gVec3 tgt, gVec3 up);

    /*!*********************************************************************
    \brief
      Calculates proj matrix based on bounds.
    \params
    \return
    ************************************************************************/
    void CalculateProjMtx();

    /*!*********************************************************************
    \brief
      Calculates the view projection matrix.
      Avoid calling this multiple times as result is not stored in object
    \params
    \return
      
    ************************************************************************/
    glm::mat4 ViewProjMtx() const;

    /*!*********************************************************************
    \brief
      Zooms camera based on supplied value multiplied by 2.
    \params
      value the value to zoom the camera divided by 2
    \return
    ************************************************************************/
    void ZoomCamera(GLfloat halfValue);

    void DisplaceCam(gVec3 displacement);
  public: // getters
    glm::mat4 const&  view{ m_view }; //!< getter to view matrix
    glm::mat4 const&  proj{ m_proj }; //!< getter to projection matrix
    gVec3 const&      position{ m_position };
    gVec3 const&      up_vector{ m_up };
  };

}
#endif

/*!*********************************************************************
\file   Camera.h
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the definition for the camera class
  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H
#include <Graphics/Def/GraphicsTypes.h>
namespace GE::Graphics::Rendering
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
    glm::mat4 m_proj;     //!< projection matrix of camera
    glm::mat4 m_pers;     //!< perspective matrix of camera
    gVec3     m_position; //!< position of camera
    gVec3     m_tgt;      //!< camera target (where are we looking?)
    gVec3     m_up;       //!< camera up vector
    GLfloat   m_left;     //!< left of camera
    GLfloat   m_right;    //!< right of camera
    GLfloat   m_bottom;   //!< bottom of camera
    GLfloat   m_top;      //!< top of camera
    GLfloat   m_near;     //!< near plane
    GLfloat   m_far;      //!< far plane
    GLfloat   m_ar;       //!< aspect ratio
    gVec2     m_dims;     //!< dimensions of camera frame (width and hight)
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
      GLfloat _near, GLfloat _far);

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
    \params
    \return
      
    ************************************************************************/
    void CalculateViewProjMtx();

    glm::mat4 const& GetPersMtx() const;
    glm::mat4 const& GetViewMtx() const;
    glm::mat4 const& GetProjMtx() const;

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
    gVec3 const& GetPos()const;
    gVec2 const& GetFrameDims()const;
  };

}
#endif

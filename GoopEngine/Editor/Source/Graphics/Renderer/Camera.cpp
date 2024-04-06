/*!*********************************************************************
\file   Camera.cpp
\author a.nalpon\@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation for the camera class
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Graphics/Renderer/Camera.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
namespace GE::Graphics::Rendering
{
  Camera::Camera(gVec3 pos, gVec3 tgt, gVec3 up,
    GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
    GLfloat _near, GLfloat _far) : m_view{ glm::lookAt(pos, tgt, up) },
    m_proj{ glm::ortho(left, right, bottom, top, _near, _far) },
    m_position{ pos },
    m_tgt{ tgt },
    m_up{ up },
    m_left{ left },
    m_right{ right },
    m_bottom{ bottom },
    m_top{ top },
    m_near{ _near },
    m_far{ _far },
    m_ar{ (m_right - m_left)/(m_top - m_bottom)},
    m_dims { m_right - m_left, m_top - m_bottom }
  {}

  Camera& Camera::operator=(Camera const& rhs)
  {
    m_position    = rhs.m_position;
    m_tgt         = rhs.m_tgt;
    m_up          = rhs.m_up;
    m_left        = rhs.m_left;
    m_right       = rhs.m_right;
    m_bottom      = rhs.m_bottom;
    m_top         = rhs.m_top;
    m_near        = rhs.m_near;
    m_far         = rhs.m_far;
    m_ar          = rhs.m_ar;
    m_dims        = rhs.m_dims;
    return *this;
  }

  void Camera::UpdateViewMtx(gVec3 pos, gVec3 tgt, gVec3 up)
  {
    m_view = glm::lookAt(pos, tgt, up);
  }

  void Camera::CalculateProjMtx()
  {
    m_proj = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
  }

  void Camera::CalculateViewProjMtx()
  {
    m_pers = m_proj * m_view;
  }

  glm::mat4 const& Camera::GetPersMtx() const
  {
    return m_pers;
  }

  glm::mat4 const& Camera::GetViewMtx() const
  {
    return m_view;
  }

  glm::mat4 const& Camera::GetProjMtx() const
  {
    return m_proj;
  }

  void Camera::ZoomCamera(GLfloat halfValue)
  {
    m_bottom += halfValue;
    m_top -= halfValue;

    // we maintain the aspect ratio by recalculating left and right ...
    GLfloat width = m_ar * (m_top - m_bottom); 
    m_right = 0.5f * width;
    m_left = -0.5f * width;
    m_dims = { width, m_top - m_bottom };
    CalculateProjMtx(); // and recalculate projection matrix based on new parameters
  }

  void Camera::DisplaceCam(gVec3 displacement)
  {
    m_position += displacement;
    m_tgt += displacement;
    UpdateViewMtx(m_position, m_tgt, m_up);
  }
  gVec3 const& Camera::GetPos() const
  {
    return m_position;
  }
  gVec2 const& Camera::GetFrameDims() const
  {
    return m_dims;
  }
}

/*!*********************************************************************
\file   Camera.cpp
\author a.nalpon@digipen.edu
\date   29-September-2023
\brief  This file contains the implementation for the camera class
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#include <pch.h>
#include <Graphics/Renderer/Camera.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
namespace Graphics::Rendering
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
    m_top{ top }
  {}

  Camera& Camera::operator=(Camera const& rhs)
  {
    m_proj = glm::mat4(rhs.proj);
    m_view  = glm::mat4(rhs.view);
    return *this;
  }

  void Camera::UpdateViewMtx(gVec3 pos, gVec3 tgt, gVec3 up)
  {
    m_view = glm::lookAt(pos, tgt, up);
  }

  glm::mat4 Camera::ViewProjMtx() const
  {
    return m_proj * m_view;
  }
  void Camera::DisplaceCam(gVec3 displacement)
  {
    m_position += displacement;
    m_tgt += displacement;
    UpdateViewMtx(m_position, m_tgt, m_up);
  }
}

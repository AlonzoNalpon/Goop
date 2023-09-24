#include <Graphics/Renderer/Camera.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
namespace Graphics::Rendering
{
  Camera::Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 up, 
    GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
    GLfloat near, GLfloat far) : m_view{ glm::lookAt(pos, tgt, up) },
              m_proj{glm::ortho(left, right, bottom, top, near, far)}
  {}

  Camera& Camera::operator=(Camera const& rhs)
  {
    m_proj = glm::mat4(rhs.proj);
    m_view  = glm::mat4(rhs.view);
    return *this;
  }

  void Camera::UpdateViewMtx(glm::vec3 pos, glm::vec3 tgt, glm::vec3 up)
  {
    m_view = glm::lookAt(pos, tgt, up);
  }
  glm::mat4 Camera::ViewProjMtx() const
  {
    return m_proj * m_view;
  }
}

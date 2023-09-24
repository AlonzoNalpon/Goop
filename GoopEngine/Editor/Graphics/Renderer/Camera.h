#ifndef CAMERA_H
#define CAMERA_H
#include <Graphics/Def/GraphicsTypes.h>
namespace Graphics::Rendering
{
  class Camera
  {
  private:
    glm::mat4 m_view;   //!< view matrix of camera
    glm::mat4 m_proj;  //!< perspective matrix of camera

  public:
    Camera() = default;
    Camera& operator=(Camera const& rhs);
    Camera(glm::vec3 pos, glm::vec3 tgt, glm::vec3 up,
      GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,
      GLfloat near, GLfloat far);
    void UpdateViewMtx(glm::vec3 pos, glm::vec3 tgt, glm::vec3 up);
    glm::mat4 ViewProjMtx() const;
  public: // getters
    glm::mat4 const& view{ m_view };
    glm::mat4 const& proj{ m_proj };
  };

}
#endif

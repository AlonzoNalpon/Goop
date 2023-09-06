#include <GLApp/Graphics/GraphicsEngine.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
namespace Graphics {
  void GraphicsEngine::Init(Color clearColor)
  {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glViewport(0, 0, 800, 800);
  }
}
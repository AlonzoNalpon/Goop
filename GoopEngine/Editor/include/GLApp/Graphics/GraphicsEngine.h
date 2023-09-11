#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H
#include <GLApp/GraphicsTypes.h>
namespace Graphics {
  // The graphics engine responsible for any opengl calls
  class GraphicsEngine {
  public:
    GraphicsEngine();
    ~GraphicsEngine();
    void Init(Color clearColor);
    Model GenerateQuad();
  protected:
    Model m_spriteQuad{};
  private:
  };
}
#endif
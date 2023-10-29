#ifndef EDITOR_VIEWPORT_H
#define EDITOR_VIEWPORT_H
#include <Graphics/Renderer/FrameBufferInfo.h>
namespace GE::EditorGUI
{
  class EditorViewport
  {
  private:
  public:
    static void UpdateViewport(Graphics::Rendering::FrameBufferInfo & fbInfo);
  };
}
#endif
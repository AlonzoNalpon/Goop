#ifndef EDITOR_VIEWPORT_H
#define EDITOR_VIEWPORT_H
#include <Graphics/Renderer/FrameBufferInfo.h>
namespace GE::EditorGUI
{
  class EditorViewport
  {
  private:
  public:
    /*!*********************************************************************
    \brief
      Updates the editor viewport. Includes input from mouse.
    \params
      fbInfo The framebuffer information that contains the scene
    \return
    ************************************************************************/
    static void UpdateViewport(Graphics::Rendering::FrameBufferInfo & fbInfo);
  };
}
#endif
/*!*********************************************************************
\file   EditorViewport.h
\author a.nalpon\@digipen.edu
\co-authors
        loh.j\@digipen.edu
\date   3-November-2023
\brief


Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
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
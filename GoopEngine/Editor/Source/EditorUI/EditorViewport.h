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
#ifndef IMGUI_DISABLE
#include <Graphics/Renderer/FrameBufferInfo.h>
#include <ECS/System/System.h>
#include <Events/EventManager.h>
namespace GE::EditorGUI
{
  class EditorViewport : public GE::Events::IEventListener, public Singleton<EditorViewport>
  {
  private:
    static bool focused;
    static bool m_deleteKeyTriggered;

    /*!*********************************************************************
    \brief
      Callback function to handle an event. Overriden from
      EventListener base class.
    \param event
      The event to handle
    ************************************************************************/
    void HandleEvent(Events::Event* event) override;

  public:
    static bool const& isFocused;

    /*!*********************************************************************
    \brief
      Updates the editor viewport. Includes input from mouse.
    \params
      fbInfo The framebuffer information that contains the scene
    \return
    ************************************************************************/
    static void UpdateViewport(Graphics::Rendering::FrameBufferInfo & fbInfo);

    static void RenderViewport(Graphics::Rendering::FrameBufferInfo& fbInfo);
  };
}
#endif
#endif

#ifndef ANIM_EVENT_EDITOR_H
#define ANIM_EVENT_EDITOR_H

#ifndef IMGUI_DISABLE
#include <Graphics/Def/GraphicsTypes.h>
namespace GE::EditorGUI
{
  class AnimEventEditor
  {
  private:
    static Graphics::gObjID currAnimationEvents; //!< currently selected in editor

  public:
    static void CreateContent();
  };
}
#endif
#endif
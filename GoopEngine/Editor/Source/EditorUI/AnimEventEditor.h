#ifndef ANIM_EVENT_EDITOR_H
#define ANIM_EVENT_EDITOR_H

#ifndef IMGUI_DISABLE
#include <Graphics/Def/GraphicsTypes.h>
#include <Component/AnimEvents.h>
namespace GE::EditorGUI
{
  using namespace Component;
  class AnimEventEditor
  {
  private:
    static std::string                currAnimationName;    //!< current animation name we are editing
    static AnimEvents::AnimEventsCont currAnimationEventsCont; //!< The container containing all the animation events
    static AnimEvents::AnimEventInfo  currAnimationEvents;        //!< current animation we are editing
    static int                        currAnimFrame; // !< frame for event to play

    static std::string                currLoadAnimEventID;      //!< the displayed string for loading anim
  public:
    static void CreateContent();
  };
}
#endif
#endif
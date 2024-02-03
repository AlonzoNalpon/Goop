#ifndef ANIM_EVENTS_SYSTEM
#define ANIM_EVENTS_SYSTEM
#include <ECS/System/System.h>

// The involved components:
#include <Component/AnimEvents.h>
namespace GE::Systems 
{
  /*!
   * \class AnimEventsSystem
   * \brief
   * System that updates and invokes events linked to animations
   */
  class AnimEventsSystem : public GE::ECS::System
  {
  public:
    /*!*********************************************************************
    \brief
      Updates and invokes animation events at component level
    \return
    ************************************************************************/
    void Update();
  };
}
#endif

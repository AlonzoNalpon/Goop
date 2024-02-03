#include <pch.h>
#include <Systems/AnimEvent/AnimEventsSystem.h>
#include <Graphics/GraphicsEngine.h>
#include <SpriteAnimation/SpriteAnimationManager.h>
#include <Events/AnimEventManager.h>
namespace GE::Systems
{
  void AnimEventsSystem::Update()
  {
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      Component::AnimEvents* animEvents{ m_ecs->GetComponent<Component::AnimEvents>(entity) };
      Component::SpriteAnim* spriteAnim{ m_ecs->GetComponent<Component::SpriteAnim>(entity) };
      auto const& animMan{ Graphics::GraphicsEngine::GetInstance().animManager };
      if (!animEvents || !spriteAnim) // skip if the event/anim components were missing
        continue;

      auto const& eventTable{ Events::AnimEventManager::GetInstance().GetAnimEventsTable() };
      for (auto const& currEventPack : animEvents->m_eventList)
      {
        if (currEventPack.first == spriteAnim->animID) // if the animation matches this event set
        {
          // We would check if current frame has events
          auto tableIt{ eventTable.find(currEventPack.second) }; // Get the matching events entry iterator
          if (tableIt == eventTable.end())
          {
            // Log error?
            continue;
          }
          // Get the iterator to container for all possible events in this sprite
          Component::AnimEvents::AnimEventsCont const& currEvtCont = tableIt->second;
          auto eventsIt = currEvtCont.find(spriteAnim->currFrame); // then we get all events in this frame (if any)
          if (eventsIt == currEvtCont.end())
            continue; // no events in this frame, skip!

          // Initialize reference to container for all events in this frame
          Component::AnimEvents::AnimFrameEvents const& currFrameEvents = eventsIt->second;

          for (std::string const& evtScript : currFrameEvents)
          {
            evtScript; // THIS IS THE SCRIPT NAME
          }
        }
      }
      
    }
  }
}

/*!*********************************************************************
\file   AnimEventsSystem.cpp
\author a.nalpon\@digipen.edu
\date   8-February-2024
\brief  This file contains the implmentation of the animation events 
system.
This system automatically calls any bound event on running frame
of animation by calling any attached script by name on mono.
   
 Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved. 
 ************************************************************************/
#include <pch.h>
#include <Systems/AnimEvent/AnimEventsSystem.h>
#include <Graphics/GraphicsEngine.h>
#include <SpriteAnimation/SpriteAnimationManager.h>
#include <Events/AnimEventManager.h>
#include <Component/Scripts.h>

namespace GE::Systems
{
  void AnimEventsSystem::Update()
  {
    for (GE::ECS::Entity entity : GetUpdatableEntities())
    {
      Component::AnimEvents* animEvents{ m_ecs->GetComponent<Component::AnimEvents>(entity) };
      Component::SpriteAnim* spriteAnim{ m_ecs->GetComponent<Component::SpriteAnim>(entity) };
      Component::Scripts* scripts{ m_ecs->GetComponent<Component::Scripts>(entity) };

      if (!animEvents || !spriteAnim) // skip if the event/anim components were missing
        continue;

      if (animEvents->m_lastFrame == spriteAnim->currFrame) // if this frame ran before, we skip
        continue;
      animEvents->m_lastFrame = spriteAnim->currFrame;  // We update if it's a new frame
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
            auto script = scripts->Get(evtScript);
            if (script)
            {
              auto method = mono_class_get_method_from_name(script->m_scriptClass, "PlayEvent", 1);
              if (method)
              {
                void* args{ &entity };
                mono_runtime_invoke(method, script->m_classInst, &args, nullptr);
              }
            }
          }
        }
      }
      
    }
  }
}

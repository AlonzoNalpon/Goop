/*!*********************************************************************
\file   AnimEvents.h
\author a.nalpon\@digipen.edu
\date   8-February-2024
\brief  This file contains the definition for Animation events
Animation events will store identifies to unique animation event sets
(in AnimEventManager). Each stored animation event set ID will
be bound to a specific animation set in the inspector.
   
 Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved. 
 ************************************************************************/
#ifndef ANIM_EVENTS_H
#define ANIM_EVENTS_H
#include <Graphics/Def/GraphicsTypes.h>
#include <map>
#include <Events/Events.h>
#include <variant>
#include <Component/Components.h>
namespace GE::Component
{
  struct AnimEvents
  {

    //enum ANIM_EVENT_TYPE
    //{
    //  PLAY_SOUND,
    //  NUM_EVENTS
    //};
    /*!
      Each animation event in the container of animation events stored HERE 
    */
    
    // Typedefs for datasets indirectly related to this component
    using AnimEventInfo   = std::string;
    using AnimFrameEvents = std::vector<AnimEventInfo>;
    using AnimEventsCont  = std::map<Graphics::gObjID, AnimFrameEvents>; //!< Frame containing a list of events (more than 1 type of event per frame?)
    using AnimEventsMap   = std::map<Graphics::gObjID, Graphics::gObjID>; //!< map for animation ID to events ID

    // Typedefs for component
    using EventEntry  = std::pair<Graphics::gObjID, std::string>;
    using EventCont   = std::vector<EventEntry>;
    /*
    using AnimEventsCont = std::map<size_t, AnimEventInfo>; // for manager
    using AnimEventsMap = std::map<Graphics::gObjID, Graphics::gObjID>; //!< map for animation ID to events ID
    */
    // DATA MEMBERS
    //AnimEventsMap m_events; //!< DO NOT SERIALIZE
    EventCont m_eventList; // !< the events list SERIALIZE
    Graphics::gObjID m_lastFrame{Graphics::BAD_OBJ_ID}; //!< the last frame that ran
  };
}

#endif

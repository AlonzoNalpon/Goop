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

    enum ANIM_EVENT_TYPE
    {
      PLAY_SOUND,
      NUM_EVENTS
    };
    /*!
      Each animation event in the container of animation events stored HERE 
    */
    using AnimEventInfo = std::string;
    
    // Typedefs
    using AnimFrameEvents = std::vector<AnimEventInfo>;
    using AnimEventsCont = std::map<Graphics::gObjID, AnimFrameEvents>; //!< Frame containing a list of events (more than 1 type of event per frame?)

    using AnimEventsMap = std::map<Graphics::gObjID, Graphics::gObjID>; //!< map for animation ID to events ID
    

    /*
    using AnimEventsCont = std::map<size_t, AnimEventInfo>; // for manager
    using AnimEventsMap = std::map<Graphics::gObjID, Graphics::gObjID>; //!< map for animation ID to events ID
    */
    // DATA MEMBERS
    AnimEventsMap m_events; //!< DO NOT SERIALIZE
    bool m_dirty{true};
  };
}

#endif

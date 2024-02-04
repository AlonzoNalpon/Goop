/*!*********************************************************************
 \file   AnimEventManager.h
 \author a.nalpon\@digipen.edu
 \date   30 January 2024
 \brief  
 
 Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved. 
 ************************************************************************/

#ifndef ANIM_EVENT_MANAGER_H
#define ANIM_EVENT_MANAGER_H
#include <Singleton/Singleton.h>
#include <Graphics/Def/GraphicsTypes.h>
#include <Component/AnimEvents.h>
namespace GE::Events
{

  /*!
  This class holds every unique event created with editor
  The AnimEventSystem will be the actual handler of these events.
  This is more of a global container
  */

  using namespace Component;
  class AnimEventManager : public Singleton<AnimEventManager>
  {
  public:
    using AnimEventsTable = std::map<std::string, AnimEvents::AnimEventsCont>;
    
    AnimEventManager();

    /*!*********************************************************************
    \brief
      Creates a set of animation events in events database. 
      May overwrite or abort based on parameters
    \param[in] name
      The name of the animation events set. Recommended to name after 
      sprite it's used for
    \param[in] eventList
      The animation event list to store
    \param[in] overwrite 
      If true, overwrites any existing events set with same name
    \return
      Status on whether adding events to entry is successful
    ************************************************************************/
    bool CreateAnimEvents(std::string const& name, 
      AnimEvents::AnimEventsCont const& eventList, bool overwrite = false);

    /*!*********************************************************************
    \brief
      Obtain a read-only reference to animation events table.
    \return
      table containing all animation events
    ************************************************************************/
    AnimEventsTable const& GetAnimEventsTable()const;

    /*!*********************************************************************
    \brief
      Obtains an animation events container with matching name. Throws
      an exception if no match can be found
    \param[in] name The name of the event to retreve. 
    \return
      
    ************************************************************************/
    AnimEvents::AnimEventsCont const& GetAnimEvent(std::string const& name)const;

    /*!*********************************************************************
    \brief
      Deletes an animation event, if it could be found
    \param name name of the animation
    \return
      flag indicating success of deletion (true if such an element was 
      found successfully deleted)
    ************************************************************************/
    bool DeleteAnimEvent(std::string const& name);
  private:
    // TODO: HOW TO SERIALIZE?
    void SaveDatabase()const;

    void LoadDatabase();

    AnimEventsTable m_animTable; //!< this table contains ALL unique events
  };
}
#endif

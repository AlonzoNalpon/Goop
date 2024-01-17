/*!*********************************************************************
\file   CardHolder.h
\author a.nalpon\@digipen.edu
\date   27-November-2023
\brief  This file contains the definition for the card holder component
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef CARD_HOLDER_H
#define CARD_HOLDER_H
#include <ECS/Entity/Entity.h>
#include <def.h>
#include <vector>
namespace GE::Component
{
  struct CardHolder
  {
    struct CardHolderEntry
    {
      GE::ECS::Entity elemEntity{ ECS::INVALID_ID }; //!< entity ID of holder element
      GE::ECS::Entity cardEntity{ ECS::INVALID_ID }; //!< entity ID of card
      GLuint  spriteID{};         //!< the id of sprite to render if used
      GLuint  defaultSpriteID{};  //!< the default sprite ID to render
      bool used{}; //!< is the slot used?
    };

    std::vector<CardHolderEntry> elements;

    GE::ECS::Entity dataEntity; //!< the entity holding the stats script
    enum DataType : unsigned int
    {
      QUEUE,
      DECK, // deck
      HAND,
      NONE
    } dataType{NONE};
    GE::ECS::Entity targetScript{ ECS::INVALID_ID };
  };
}

#endif

/*!*********************************************************************
\file   CardHolderElem.h
\author a.nalpon\@digipen.edu
\date   27-November-2023
\brief  This file contains the definition of the card holder element
component
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef CARD_HOLDER_ELEM_H
#define CARD_HOLDER_ELEM_H

#include <ECS/Entity/Entity.h>

namespace GE::Component
{
  /*!
   * \brief  
   * Card holder elements are elements of a card holder.
   */
  struct CardHolderElem
  {
    ECS::Entity holder{ECS::INVALID_ID};
    size_t elemIdx{ECS::INVALID_ID}; // !< which element in the container of holder
  };
}

#endif

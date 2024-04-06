/*!*********************************************************************
\file   Card.h
\author a.nalpon\@digipen.edu
\date   27-November-2023
\brief  This file contains the enums used for cards in the game. They
				follow the same order in the C# side of gameplay.
  
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
#ifndef CARD_H
#define CARD_H

#include <ECS/Entity/Entity.h>
namespace GE::Component
{
  /*!
   * \brief  
   * Card struct to store card type and a card holder ID.
   */
  struct Card
  {
		enum CardID : unsigned int
		{
			NO_CARD,

			// Basic enemy cards
			BASIC_ATTACK,
			BASIC_SHIELD,
			SPECIAL_SCREECH,

			// Player cards
			LEAH_BEAM,
			LEAH_STRIKE,
			LEAH_SHIELD,

			SPECIAL_FLASHBANG,
			SPECIAL_SMOKESCREEN,
			SPECIAL_RAGE,

			// Dawson enemy cards
			DAWSON_BEAM,
			DAWSON_SWING,
			DAWSON_SHIELD,

			SPECIAL_CHARGEUP,
			SPECIAL_TIMEWRAP,

			TOTAL_CARDS,
		};
		CardID cardID{};	// The card ID that identifies the type of card (to interact with C#)
		GE::ECS::Entity tgtEntity{ GE::ECS::INVALID_ID }; //!< an entity ID. Possibly for card holder
	};
}

#endif

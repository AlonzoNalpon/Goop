/*!*********************************************************************
\file   Card.h
\author a.nalpon\@digipen.edu
\date   27-November-2023
\brief  Card component to store card ID of card and target entity
				(like a holder). See GameDef.h for additional info
				on the struct
  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
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
		enum CardID : int
		{
			NO_CARD, 

			// Basic enemy cards
			BASIC_ATTACK,
			BASIC_SHIELD,
			BASIC_BUFF_SCREECH,

			// Player cards
			PLAYER_ATTACK_T1,
			PLAYER_ATTACK_T2,

			PLAYER_SHIELD,

			PLAYER_DEBUFF_FLASH_BANG,
			PLAYER_BUFF_SMOKESCREEN,
			PLAYER_BUFF_RAGE,

			// Dawson enemy cards
			DAWSON_ATTACK_T1,
			DAWSON_ATTACK_T2,

			DAWSON_SHIELD,

			DAWSON_BUFF_CHARGE_UP,
			DAWSON_DEBUFF_TIME_WARP,

			TOTAL_CARDS,
		};
		CardID cardID{};	// The card ID that identifies the type of card (to interact with C#)
		GE::ECS::Entity tgtEntity{ GE::ECS::INVALID_ID }; //!< an entity ID. Possibly for card holder
	};
}

#endif

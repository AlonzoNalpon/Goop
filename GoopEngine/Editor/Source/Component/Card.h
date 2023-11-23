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
			BASIC_NORMAL_ATTACK,
			BASIC_NORMAL_SHIELD,

			BASIC_NORMAL_BUFF,

			// Player cards
			PLAYER_BEAM_ATTACK,
			PLAYER_STRIKE_ATTACK,

			PLAYER_NORMAL_SHIELD,

			PLAYER_DEBUFF_FLASH_BANG,
			PLAYER_BUFF_SMOKESCREEN,
			PLAYER_BUFF_RAGE,

			// Dawson enemy cards
			DAWSON_BEAM_ATTACK,
			DAWSON_STRIKE_ATTACK,

			DAWSON_NORMAL_SHIELD,

			DAWSON_BUFF_CHARGE_UP,
			DAWSON_DEBUFF_TIME_WARP,

			TOTAL_CARDS,
		};
		CardID cardID{};
		GE::ECS::Entity tgtEntity{ GE::ECS::INVALID_ID }; //!< an entity ID. Possibly for card holder
	};
}

#endif

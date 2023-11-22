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
			// Basic enemy cards
			BASIC_LUNGE_ATTACK,
			BASIC_NORMAL_ATTACK,

			BASIC_NORMAL_BLOCK,

			BASIC_BUFF_2X_ATTACK,

			// Player cards
			PLAYER_BEAM_ATTACK,
			PLAYER_EMPOWERED_ATTACK,
			PLAYER_EMPOWERED_SWING,

			PLAYER_NORMAL_BLOCK,

			PLAYER_DEBUFF_FLASH_BANG,
			PLAYER_BUFF_SMOKESCREEN,
			PLAYER_BUFF_RAGE,
			PLAYER_EMPOWERED_BEAM_ATTACK,

			// Dawson enemy cards
			DAWSON_BEAM_ATTACK,
			DAWSON_EMPOWERED_ATTACK,

			DAWSON_NORMAL_BLOCK,

			DAWSON_DEBUFF_CHARGE_ATTACK,
			DAWSON_DEBUFF_SPEED_UP_QUEUE,

			TOTAL_CARDS,
		};
		CardID cardID{};
		GE::ECS::Entity tgtEntity{ GE::ECS::INVALID_ID }; //!< an entity ID. Possibly for card holder
	};
}

#endif

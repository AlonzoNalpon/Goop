#ifndef GAME_DEF_H
#define GAME_DEF_H
#include <utility>
#include <array>
#include <Component/Card.h>
namespace GE
{

  inline constexpr std::array<const char*, Component::Card::CardID::TOTAL_CARDS> CardSpriteNames
  {
    "DAA2402_CardBaseBack_v01",									  // NO_CARD 
																								
		//____________________________________________ BASIC ENEMY CARDS ______________________
    "DAA2402_Card_BasicAttack_ClawSwipe_v01",						// BASIC_NORMAL_ATTACK
    "DAA2402_Card_BasicDefense_Shield_v01",						// BASIC_NORMAL_SHIELD

    "DAA2402_Card_BasicSpecial_Screech_v01",				  // BASIC_NORMAL_BUFF

		//____________________________________________ PLAYER CARDS ___________________________
    "DAA2402_Card_LeahAttack_BeamShot_v02",						  // PLAYER_BEAM_ATTACK
    "DAA2402_Card_LeahAttack_Slash_v02",						    // PLAYER_STRIKE_ATTACK

    "DAA2402_Card_LeahDefense_Shield_v01",						// PLAYER_NORMAL_SHIELD

    "DAA2402_Card_LeahSpecial_FlashBang_v02",						  // PLAYER_DEBUFF_FLASH_BANG
    "DAA2402_Card_LeahSpecial_Smokescreen_v01",						// PLAYER_BUFF_SMOKESCREEN
    "DAA2402_Card_LeahSpecial_Berserk_v01",						// PLAYER_BUFF_RAGE

		//____________________________________________ DAWSON ENEMY CARDS _____________________
    "DAA2402_Card_BossAttack_BeamShot_v01",						// DAWSON_BEAM_ATTACK
    "DAA2402_Card_BossAttack_Strike_v01",						// DAWSON_STRIKE_ATTACK

    "DAA2402_Card_BossDefense_Deflect_v01",						// DAWSON_NORMAL_SHIELD

    "DAA2402_Card_BossSpecial_ChargeUp_v01",						// DAWSON_BUFF_CHARGE_UP
    "DAA2402_Card_BossSpecial_TimeWarp_v01",						// DAWSON_DEBUFF_TIME_WARP
  };
}

#endif

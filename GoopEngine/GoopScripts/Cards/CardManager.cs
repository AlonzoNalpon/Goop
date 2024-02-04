/*!*********************************************************************
\file   CardManager.cs
\author w.chinkitbryan\@digipen.edu
\date   26-November-2023
\brief  
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/
using GoopScripts.Gameplay;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;


namespace GoopScripts.Cards
{
	public static class CardManager
	{
    static Dictionary<CardBase.CardID, CardBase> m_cards = new Dictionary<CardBase.CardID, CardBase>
    {
      { CardBase.CardID.NO_CARD, new BlankCard(CardBase.CardID.NO_CARD, CardBase.CardType.BLANK_CARD) },

      // normal enemy
      { CardBase.CardID.BASIC_ATTACK, new BlankCard(CardBase.CardID.BASIC_ATTACK, CardBase.CardType.ATTACK) },
      { CardBase.CardID.BASIC_SHIELD, new BlankCard(CardBase.CardID.BASIC_SHIELD, CardBase.CardType.BLOCK) },
      { CardBase.CardID.SPECIAL_SCREECH, new BlankCard(CardBase.CardID.SPECIAL_SCREECH, CardBase.CardType.SPECIAL) },

      // player
      { CardBase.CardID.LEAH_BEAM, new BlankCard(CardBase.CardID.LEAH_BEAM, CardBase.CardType.ATTACK) },
      { CardBase.CardID.LEAH_STRIKE, new BlankCard(CardBase.CardID.LEAH_STRIKE, CardBase.CardType.ATTACK) },
      { CardBase.CardID.LEAH_SHIELD, new BlankCard(CardBase.CardID.LEAH_SHIELD, CardBase.CardType.BLOCK) },
      { CardBase.CardID.SPECIAL_FLASHBANG, new BlankCard(CardBase.CardID.SPECIAL_FLASHBANG, CardBase.CardType.SPECIAL) },
      { CardBase.CardID.SPECIAL_SMOKESCREEN, new BlankCard(CardBase.CardID.SPECIAL_SMOKESCREEN, CardBase.CardType.SPECIAL) },
      { CardBase.CardID.SPECIAL_RAGE, new BlankCard(CardBase.CardID.SPECIAL_RAGE, CardBase.CardType.SPECIAL) },

      // dawson
      { CardBase.CardID.DAWSON_BEAM, new BlankCard(CardBase.CardID.DAWSON_BEAM, CardBase.CardType.ATTACK) },
      { CardBase.CardID.DAWSON_SWING, new BlankCard(CardBase.CardID.DAWSON_SWING, CardBase.CardType.ATTACK) },
      { CardBase.CardID.DAWSON_SHIELD, new BlankCard(CardBase.CardID.DAWSON_SHIELD, CardBase.CardType.BLOCK) },
      { CardBase.CardID.SPECIAL_CHARGEUP, new BlankCard(CardBase.CardID.SPECIAL_CHARGEUP, CardBase.CardType.SPECIAL) },
      { CardBase.CardID.SPECIAL_TIMEWRAP, new BlankCard(CardBase.CardID.SPECIAL_TIMEWRAP, CardBase.CardType.SPECIAL) }
    };

    // HARDCODED FOR NOW - Reason below
    // Discrete Math Reference - If Prefab names won't change, then values here won't need to change
    static public readonly Dictionary<CardBase.CardID, string> m_cardPrefabs = new Dictionary<CardBase.CardID, string>
    {
      // basic enemy
      { CardBase.CardID.BASIC_ATTACK, "Normal_Attack"  },
      { CardBase.CardID.BASIC_SHIELD, "Normal_Block" },
      { CardBase.CardID.SPECIAL_SCREECH, "Normal_Screech" },

      // player
      { CardBase.CardID.LEAH_BEAM, "Leah_Attack" },
      { CardBase.CardID.LEAH_STRIKE, "Leah_Attack2" },
      { CardBase.CardID.LEAH_SHIELD, "Leah_Block" },
      { CardBase.CardID.SPECIAL_FLASHBANG, "Leah_Flashbang" },
      { CardBase.CardID.SPECIAL_SMOKESCREEN, "Leah_Smokescreen" },
      { CardBase.CardID.SPECIAL_RAGE, "Leah_Berserk" },

      // dawson
      { CardBase.CardID.DAWSON_BEAM, "Dawson_Attack" },
      { CardBase.CardID.DAWSON_SWING, "Dawson_Attack2" },
      { CardBase.CardID.DAWSON_SHIELD, "Dawson_Block" },
      { CardBase.CardID.SPECIAL_CHARGEUP, "Dawson_ChargeUp" },
      { CardBase.CardID.SPECIAL_TIMEWRAP, "Dawson_TimeWarp" }
    };

    static CardManager()
		{
      
    }

    static public CardBase Get(CardBase.CardID id)
    {
      return m_cards[id];
    }
  }

}


//static string[] CardSpriteNames = new string[]
//{
//  "DAA2402_CardBaseBack_v01",									  // NO_CARD 
////____________________________________________ BASIC ENEMY CARDS ______________________
//  "DAA2402_Card_BasicAttack_ClawSwipe_v01",						// BASIC_NORMAL_ATTACK
//  "DAA2402_Card_BasicDefense_Shield_v01",						// BASIC_NORMAL_SHIELD

//  "DAA2402_Card_BasicSpecial_Screech_v01",				  // BASIC_NORMAL_BUFF
////____________________________________________ PLAYER CARDS ___________________________
//  "DAA2402_Card_LeahAttack_BeamShot_v02",						  // PLAYER_BEAM_ATTACK
//  "DAA2402_Card_LeahAttack_Slash_v02",						    // PLAYER_STRIKE_ATTACK
//  "DAA2402_Card_LeahDefense_Shield_v01",						// PLAYER_NORMAL_SHIELD
//  "DAA2402_Card_LeahSpecial_FlashBang_v02",						  // PLAYER_DEBUFF_FLASH_BANG
//  "DAA2402_Card_LeahSpecial_Smokescreen_v01",						// PLAYER_BUFF_SMOKESCREEN
//  "DAA2402_Card_LeahSpecial_Berserk_v01",						// PLAYER_BUFF_RAGE
////____________________________________________ DAWSON ENEMY CARDS _____________________
//  "DAA2402_Card_BossAttack_BeamShot_v01",						// DAWSON_BEAM_ATTACK
//  "DAA2402_Card_BossAttack_Strike_v01",						// DAWSON_STRIKE_ATTACK
//  "DAA2402_Card_BossDefense_Deflect_v01",						// DAWSON_NORMAL_SHIELD
//  "DAA2402_Card_BossSpecial_ChargeUp_v01",						// DAWSON_BUFF_CHARGE_UP
//  "DAA2402_Card_BossSpecial_TimeWarp_v01",						// DAWSON_DEBUFF_TIME_WARP
//};
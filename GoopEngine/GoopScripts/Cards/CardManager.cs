/*!*********************************************************************
\file   CardManager.cs
\author w.chinkitbryan\@digipen.edu
\co-author c.phua\@digipen.edu
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
using System.IO;


namespace GoopScripts.Cards
{
	public static class CardManager
	{
    static public readonly string CARD_ICON_PREFAB = "CardIcon";

    static Dictionary<CardBase.CardID, CardBase> m_cards;

    // HARDCODED FOR NOW - Reason below
    // Discrete Math Reference - If Prefab names won't change, then values here won't need to change
    // This statement is True
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

    static public readonly Dictionary<CardBase.CardID, string> m_cardIcons = new Dictionary<CardBase.CardID, string>
    {
      // basic enemy
      { CardBase.CardID.BASIC_ATTACK, "CardIcon_BasicAttackClawswipe"  },
      { CardBase.CardID.BASIC_SHIELD, "CardIcon_BasicDefenceShield" },
      { CardBase.CardID.SPECIAL_SCREECH, "CardIcon_BasicSpecialScreech" },

      // player
      { CardBase.CardID.LEAH_BEAM, "CardIcon_LeahAttackBeam" },
      { CardBase.CardID.LEAH_STRIKE, "CardIcon_LeahAttackSlash" },
      { CardBase.CardID.LEAH_SHIELD, "CardIcon_LeahDefenceShield" },
      { CardBase.CardID.SPECIAL_FLASHBANG, "CardIcon_LeahSpecialFlashbang" },
      { CardBase.CardID.SPECIAL_SMOKESCREEN, "CardIcon_LeahSpecialSmokescreen" },
      { CardBase.CardID.SPECIAL_RAGE, "CardIcon_LeahSpecialBerserk" },

      // dawson
      { CardBase.CardID.DAWSON_BEAM, "CardIcon_BossAttackBeamshot" },
      { CardBase.CardID.DAWSON_SWING, "CardIcon_BossAttackStrike" },
      { CardBase.CardID.DAWSON_SHIELD, "CardIcon_BossDefenceDeflect" },
      { CardBase.CardID.SPECIAL_CHARGEUP, "CardIcon_BossSpecialChargeup" },
      { CardBase.CardID.SPECIAL_TIMEWRAP, "CardIcon_BossSpecialTimewarp" }
    };


    static CardManager()
    {
      string filePath = "./Assets/Animations.txt";
      // Process each line and extract the ID (stop at the space character)
      Dictionary<int, string> cardIDtoSpriteMap = new Dictionary<int, string>();
      try
      {
        // Read all lines from the file
        string[] lines = File.ReadAllLines(filePath);
        int count = 0;

        foreach (string line in lines)
        {
          string trimmedLine = line.Trim();
          // Find the position of the space character
          int spaceIndex = trimmedLine.IndexOf(' ');

          // Extract the ID and Value based on the space
          if (spaceIndex >= 0)
          {
            string spriteName = trimmedLine.Substring(0, spaceIndex);
            cardIDtoSpriteMap[count] = spriteName;
          }
          ++count;
        }

        m_cards = new Dictionary<CardBase.CardID, CardBase>
        {
          { CardBase.CardID.NO_CARD, new BlankCard(CardBase.CardID.NO_CARD, CardBase.CardType.BLANK_CARD,cardIDtoSpriteMap[(int)CardBase.CardID.NO_CARD]) },

          // normal enemy
          { CardBase.CardID.BASIC_ATTACK, new BasicAttack(CardBase.CardID.BASIC_ATTACK, CardBase.CardType.ATTACK, cardIDtoSpriteMap[(int)CardBase.CardID.BASIC_ATTACK]) },
          { CardBase.CardID.BASIC_SHIELD, new Block(CardBase.CardID.BASIC_SHIELD, CardBase.CardType.BLOCK, cardIDtoSpriteMap[(int)CardBase.CardID.BASIC_SHIELD]) },
          { CardBase.CardID.SPECIAL_SCREECH, new SpecialScreech(CardBase.CardID.SPECIAL_SCREECH, CardBase.CardType.SPECIAL, cardIDtoSpriteMap[(int)CardBase.CardID.SPECIAL_SCREECH]) },

          // player
          { CardBase.CardID.LEAH_BEAM, new LeahBeam(CardBase.CardID.LEAH_BEAM, CardBase.CardType.ATTACK, cardIDtoSpriteMap[(int)CardBase.CardID.LEAH_BEAM]) },
          { CardBase.CardID.LEAH_STRIKE, new LeahStrike(CardBase.CardID.LEAH_STRIKE, CardBase.CardType.ATTACK, cardIDtoSpriteMap[(int)CardBase.CardID.LEAH_STRIKE]) },
          { CardBase.CardID.LEAH_SHIELD, new Block(CardBase.CardID.LEAH_SHIELD, CardBase.CardType.BLOCK, cardIDtoSpriteMap[(int)CardBase.CardID.LEAH_SHIELD]) },
          { CardBase.CardID.SPECIAL_FLASHBANG, new SpecialFlashBang(CardBase.CardID.SPECIAL_FLASHBANG, CardBase.CardType.SPECIAL, cardIDtoSpriteMap[(int)CardBase.CardID.SPECIAL_FLASHBANG]) },
          { CardBase.CardID.SPECIAL_SMOKESCREEN, new SpecialFlashBang(CardBase.CardID.SPECIAL_SMOKESCREEN, CardBase.CardType.SPECIAL, cardIDtoSpriteMap[(int)CardBase.CardID.SPECIAL_SMOKESCREEN]) },
          { CardBase.CardID.SPECIAL_RAGE, new SpecialRage(CardBase.CardID.SPECIAL_RAGE, CardBase.CardType.SPECIAL, cardIDtoSpriteMap[(int)CardBase.CardID.SPECIAL_RAGE]) },

          // dawson
          { CardBase.CardID.DAWSON_BEAM, new DawsonBeam(CardBase.CardID.DAWSON_BEAM, CardBase.CardType.ATTACK, cardIDtoSpriteMap[(int)CardBase.CardID.DAWSON_BEAM]) },
          { CardBase.CardID.DAWSON_SWING, new DawsonSwing(CardBase.CardID.DAWSON_SWING, CardBase.CardType.ATTACK, cardIDtoSpriteMap[(int)CardBase.CardID.DAWSON_SWING]) },
          { CardBase.CardID.DAWSON_SHIELD, new Block(CardBase.CardID.DAWSON_SHIELD, CardBase.CardType.BLOCK, cardIDtoSpriteMap[(int)CardBase.CardID.DAWSON_SHIELD]) },
          { CardBase.CardID.SPECIAL_CHARGEUP, new SpecialChargeUp(CardBase.CardID.SPECIAL_CHARGEUP, CardBase.CardType.SPECIAL, cardIDtoSpriteMap[(int)CardBase.CardID.SPECIAL_CHARGEUP]) },
          { CardBase.CardID.SPECIAL_TIMEWRAP, new SpecialTimeWrap(CardBase.CardID.SPECIAL_TIMEWRAP, CardBase.CardType.SPECIAL, cardIDtoSpriteMap[(int)CardBase.CardID.SPECIAL_TIMEWRAP]) }
        };
      }
      catch (Exception ex)
      {
        Console.WriteLine($"Error reading file: {ex.Message}");
      }
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
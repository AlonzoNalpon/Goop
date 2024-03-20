/*!*********************************************************************
\file   CardManager.cs
\author w.chinkitbryan\@digipen.edu
\co-author c.phua\@digipen.edu
\date   26-November-2023
\brief  
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
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
      { CardBase.CardID.SPECIAL_TIMEWARP, "Dawson_TimeWarp" }
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
      { CardBase.CardID.SPECIAL_TIMEWARP, "CardIcon_BossSpecialTimewarp" }
    };


    static CardManager()
    {
      try
      { 
        m_cards = new Dictionary<CardBase.CardID, CardBase>
        {
          { CardBase.CardID.NO_CARD, new BlankCard(CardBase.CardID.NO_CARD, CardBase.CardType.BLANK_CARD) },

          // normal enemy
          { CardBase.CardID.BASIC_ATTACK, new BasicAttack(CardBase.CardID.BASIC_ATTACK, CardBase.CardType.ATTACK) },
          { CardBase.CardID.BASIC_SHIELD, new Block(CardBase.CardID.BASIC_SHIELD, CardBase.CardType.BLOCK) },
          { CardBase.CardID.SPECIAL_SCREECH, new SpecialScreech(CardBase.CardID.SPECIAL_SCREECH, CardBase.CardType.SPECIAL) },

          // player
          { CardBase.CardID.LEAH_BEAM, new LeahBeam(CardBase.CardID.LEAH_BEAM, CardBase.CardType.ATTACK) },
          { CardBase.CardID.LEAH_STRIKE, new LeahStrike(CardBase.CardID.LEAH_STRIKE, CardBase.CardType.ATTACK) },
          { CardBase.CardID.LEAH_SHIELD, new Block(CardBase.CardID.LEAH_SHIELD, CardBase.CardType.BLOCK) },
          { CardBase.CardID.SPECIAL_FLASHBANG, new SpecialFlashBang(CardBase.CardID.SPECIAL_FLASHBANG, CardBase.CardType.SPECIAL) },
          { CardBase.CardID.SPECIAL_SMOKESCREEN, new SpecialSmokeScreen(CardBase.CardID.SPECIAL_SMOKESCREEN, CardBase.CardType.SPECIAL) },
          { CardBase.CardID.SPECIAL_RAGE, new SpecialRage(CardBase.CardID.SPECIAL_RAGE, CardBase.CardType.SPECIAL) },

          // dawson
          { CardBase.CardID.DAWSON_BEAM, new DawsonBeam(CardBase.CardID.DAWSON_BEAM, CardBase.CardType.ATTACK) },
          { CardBase.CardID.DAWSON_SWING, new DawsonSwing(CardBase.CardID.DAWSON_SWING, CardBase.CardType.ATTACK) },
          { CardBase.CardID.DAWSON_SHIELD, new Block(CardBase.CardID.DAWSON_SHIELD, CardBase.CardType.BLOCK) },
          { CardBase.CardID.SPECIAL_CHARGEUP, new SpecialChargeUp(CardBase.CardID.SPECIAL_CHARGEUP, CardBase.CardType.SPECIAL) },
          { CardBase.CardID.SPECIAL_TIMEWARP, new SpecialTimeWrap(CardBase.CardID.SPECIAL_TIMEWARP, CardBase.CardType.SPECIAL) }
        };
      }
#if (DEBUG)
      catch (Exception ex)
      {
        Console.WriteLine($"Error reading file: {ex.Message}");
      }
#else
      catch (Exception)
      {

      }
#endif
  }

    static public CardBase Get(CardBase.CardID id)
    {
      return m_cards[id];
    }
  }

}

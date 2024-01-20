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
		static CardBase[] m_cards;

		/*!*********************************************************************
		\brief
      Default constructor of all cards. Creates an array of all existing
      cards to then later be used for as a lookup during game logic processing
		************************************************************************/
		static CardManager()
		{
      // All cards calling TakeDamage() function currently causes crashes.
      // The function has not yet been properly implemented for gameplay
      m_cards = new CardBase[(int)CardBase.CardID.TOTAL_CARDS];

      m_cards[(int)CardBase.CardID.NO_CARD] = new BlankCard(CardBase.CardID.NO_CARD);

      //normal enemy
      m_cards[(int)CardBase.CardID.BASIC_ATTACK] = new BasicAttack(CardBase.CardID.BASIC_ATTACK);
      m_cards[(int)CardBase.CardID.BASIC_SHIELD] = new Block(CardBase.CardID.BASIC_SHIELD);
      m_cards[(int)CardBase.CardID.SPECIAL_SCREECH] = new SpecialScreech(CardBase.CardID.SPECIAL_SCREECH);

      //player
      m_cards[(int)CardBase.CardID.LEAH_BEAM] = new LeahBeam(CardBase.CardID.LEAH_BEAM);
      m_cards[(int)CardBase.CardID.LEAH_STRIKE] = new LeahStrike(CardBase.CardID.LEAH_STRIKE);
      m_cards[(int)CardBase.CardID.LEAH_SHIELD] = new Block(CardBase.CardID.LEAH_SHIELD);
      m_cards[(int)CardBase.CardID.SPECIAL_FLASHBANG] = new SpecialFlashBang(CardBase.CardID.SPECIAL_FLASHBANG);
      m_cards[(int)CardBase.CardID.SPECIAL_SMOKESCREEN] = new SpecialSmokeScreen(CardBase.CardID.SPECIAL_SMOKESCREEN);
      m_cards[(int)CardBase.CardID.SPECIAL_RAGE] = new SpecialRage(CardBase.CardID.SPECIAL_RAGE);

      //dawson
      m_cards[(int)CardBase.CardID.DAWSON_BEAM] = new DawsonBeam(CardBase.CardID.DAWSON_BEAM);
      m_cards[(int)CardBase.CardID.DAWSON_SWING] = new DawsonSwing(CardBase.CardID.DAWSON_SWING);
      m_cards[(int)CardBase.CardID.DAWSON_SHIELD] = new Block(CardBase.CardID.DAWSON_SHIELD);
      m_cards[(int)CardBase.CardID.SPECIAL_CHARGEUP] = new SpecialChargeUp(CardBase.CardID.SPECIAL_CHARGEUP);
      m_cards[(int)CardBase.CardID.SPECIAL_TIMEWRAP] = new SpecialTimeWrap(CardBase.CardID.SPECIAL_TIMEWRAP);
    }

    public static CardBase Get(CardBase.CardID id)
    {
      return m_cards[(int) id];
    }
  }
}

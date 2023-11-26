/*!*********************************************************************
\file   DawsonChargeUp.cs
\author w.chinkitbryan\@digipen.edu
\date   26-November-2023
\brief  
  DawsonChargeUp card, derived from card base.
 
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
	internal class CardManager
	{
		CardBase[] m_cards;
		public CardBase[] Cards { get { return m_cards; } set { m_cards = value; } }

		/*!*********************************************************************
		\brief
      Default constructor of all cards. Creates an array of all existing
      cards to then later be used for as a lookup during game logic processing
		************************************************************************/
		public CardManager()
		{
      // All cards calling TakeDamage() function currently causes crashes.
      // The function has not yet been properly implemented for gameplay
      m_cards = new CardBase[(int)CardBase.CardID.TOTAL_CARDS];

      m_cards[(int)CardBase.CardID.NO_CARD] = new BlankCard(CardBase.CardID.NO_CARD);

      m_cards[(int)CardBase.CardID.BASIC_NORMAL_ATTACK] = new Attack(CardBase.CardID.BASIC_NORMAL_ATTACK);
      m_cards[(int)CardBase.CardID.BASIC_NORMAL_SHIELD] = new Block(CardBase.CardID.BASIC_NORMAL_SHIELD);
      m_cards[(int)CardBase.CardID.BASIC_NORMAL_BUFF] = new Rage(CardBase.CardID.BASIC_NORMAL_BUFF);
      m_cards[(int)CardBase.CardID.PLAYER_BEAM_ATTACK] = new Attack(CardBase.CardID.PLAYER_BEAM_ATTACK);
      m_cards[(int)CardBase.CardID.PLAYER_STRIKE_ATTACK] = new Attack(CardBase.CardID.PLAYER_STRIKE_ATTACK);
      m_cards[(int)CardBase.CardID.PLAYER_NORMAL_SHIELD] = new Block(CardBase.CardID.PLAYER_NORMAL_SHIELD);
      m_cards[(int)CardBase.CardID.PLAYER_DEBUFF_FLASH_BANG] = new LeahFlashBang(CardBase.CardID.PLAYER_DEBUFF_FLASH_BANG);
      m_cards[(int)CardBase.CardID.PLAYER_BUFF_SMOKESCREEN] = new LeahSmokeScreen(CardBase.CardID.PLAYER_BUFF_SMOKESCREEN);
      m_cards[(int)CardBase.CardID.PLAYER_BUFF_RAGE] = new Rage(CardBase.CardID.PLAYER_BUFF_RAGE);
      m_cards[(int)CardBase.CardID.DAWSON_BEAM_ATTACK] = new Attack(CardBase.CardID.DAWSON_BEAM_ATTACK);
      m_cards[(int)CardBase.CardID.DAWSON_STRIKE_ATTACK] = new Attack(CardBase.CardID.DAWSON_STRIKE_ATTACK);
      m_cards[(int)CardBase.CardID.DAWSON_NORMAL_SHIELD] = new Block(CardBase.CardID.DAWSON_NORMAL_SHIELD);
      m_cards[(int)CardBase.CardID.DAWSON_BUFF_CHARGE_UP] = new DawsonChargeUp(CardBase.CardID.DAWSON_BUFF_CHARGE_UP);
      // change this class lol
      m_cards[(int)CardBase.CardID.DAWSON_DEBUFF_TIME_WARP] = new DawsonSpeedUp(CardBase.CardID.DAWSON_DEBUFF_TIME_WARP);
    }
  }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;
using GoopScripts.Mono;

namespace GoopScripts.Gameplay
{
	public  class Stats
	{
		public int m_health=100;
    // List of card ID

    public CardBase.CardID[] m_deck = { };
		// List of cards in the queue
		public CardBase.CardID[] m_cardQueue = { };

    public CardBase.CardID[] m_hand = { };

    public BuffManager m_buffs { get; set; }

    // Hard coded for testing
    // cards are not properly implemented yet
    Random rng;
    public Stats()
		{
      rng = new Random();

			m_deck = new CardBase.CardID[20];
      for (int i = 0; i < m_deck.Length; ++i)
      {
        switch (rng.Next(0, 8))
        {
          case 0:
						m_deck[i] = CardBase.CardID.BASIC_NORMAL_SHIELD;
						break;
					case 1:
						m_deck[i] = CardBase.CardID.BASIC_NORMAL_BUFF;
						break;
          case 2:
						m_deck[i] = CardBase.CardID.PLAYER_NORMAL_SHIELD;
						break;
          case 3:
						m_deck[i] = CardBase.CardID.PLAYER_DEBUFF_FLASH_BANG;
						break;
          case 4:
						m_deck[i] = CardBase.CardID.PLAYER_BUFF_SMOKESCREEN;
						break;
          case 5:
						m_deck[i] = CardBase.CardID.PLAYER_BUFF_RAGE;
						break;  
          case 6:
						m_deck[i] = CardBase.CardID.DAWSON_NORMAL_SHIELD;
						break; 
          case 7:
						m_deck[i] = CardBase.CardID.DAWSON_BUFF_CHARGE_UP;
						break; 
          case 8:
						m_deck[i] = CardBase.CardID.DAWSON_DEBUFF_TIME_WARP;
						break;
        }
			}
      
			m_cardQueue = new CardBase.CardID[3];
      // Attack cards causes crash
      m_cardQueue[0] = CardBase.CardID.NO_CARD;
      m_cardQueue[1] = CardBase.CardID.NO_CARD;
      m_cardQueue[2] = CardBase.CardID.NO_CARD;

			m_hand = new CardBase.CardID[5];
      m_hand[0] = CardBase.CardID.PLAYER_DEBUFF_FLASH_BANG;
      m_hand[1] = CardBase.CardID.BASIC_NORMAL_BUFF;
      m_hand[2] = CardBase.CardID.PLAYER_BUFF_SMOKESCREEN;
      m_hand[3] = CardBase.CardID.BASIC_NORMAL_SHIELD;
      m_hand[4] = CardBase.CardID.BASIC_NORMAL_SHIELD;
		}

    public int GetHealth()
    {
      return m_health;
    }

    public void TakeDamage(float damage)
		{
			// Value can be negative and < 0 means you take more dmg
			float takenMultiplier = 0;
      int takenFlat = 0;
			foreach (var buff in m_buffs.Buffs)
			{
				switch(buff.type)
				{
          case Buff.BuffType.INCREASE_ATK_TAKEN:
            takenFlat += (int)buff.value;
            break;
          case Buff.BuffType.REDUCE_DMG_TAKEN:
            takenFlat -= (int)buff.value;
            break;
          case Buff.BuffType.PER_DMG_TAKEN:
            takenMultiplier += buff.value;
            break;
					default:
						break;
        }
			}

			int dmgTaken = (int)((damage - takenFlat) * takenMultiplier);
			dmgTaken = dmgTaken < 0 ? 0 : dmgTaken;

			m_health -= dmgTaken;
    }
	
		public int DamageDealt(float damage)
		{
      // Value can be negative and < 0 means you take more dmg
      float dealtMultiplier = 0;
      int dealtFlat = 0;
      foreach (var buff in m_buffs.Buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.INCREASE_ATK_DEALT:
            dealtFlat += (int)buff.value;
            break;
          case Buff.BuffType.REDUCE_ATK_DEALT:
            dealtFlat -= (int)buff.value;
            break;
          case Buff.BuffType.PER_DMG_DEALT:
            dealtMultiplier += buff.value;
            break;
          default:
            break;
        }
      }

      int dmgDealt = (int)((damage - dealtFlat) * dealtMultiplier);
      dmgDealt = dmgDealt < 0 ? 0 : dmgDealt;

      return dmgDealt;
    }

    public void SetCardInHand(int index, CardBase.CardID cardID)
    {
      m_hand[index] = cardID;
    }

    public void SetCardInQueue(int index, CardBase.CardID cardID)
    {
      m_cardQueue[index] = cardID;
    }
  }
}

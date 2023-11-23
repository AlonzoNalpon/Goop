using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;


namespace GoopScripts.Gameplay
{
	public  class Stats
	{
		public int m_health=100;
		// List of card IDs
		public CardBase.CardID[] m_deck { get; set; }  // HQD: I had to initialize them, or else they would gives me error. you can remove it
		// List of cards in the queue
		public CardBase.CardID[] m_cardQueue { get; set; }

    public CardBase.CardID[] m_hand { get; set; }

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
        m_deck[i] = (CardBase.CardID)rng.Next(1, (int)CardBase.CardID.TOTAL_CARDS);
			}
      
			m_cardQueue = new CardBase.CardID[3];
      // Attack cards causes crash
      m_cardQueue[0] = CardBase.CardID.BASIC_NORMAL_SHIELD;
      m_cardQueue[1] = CardBase.CardID.BASIC_NORMAL_BUFF;
      m_cardQueue[2] = CardBase.CardID.PLAYER_DEBUFF_FLASH_BANG;

			m_hand = new CardBase.CardID[5];
			for (int i = 0; i < m_hand.Length; ++i)
			{
				m_hand[i] = (CardBase.CardID)rng.Next(1, (int)CardBase.CardID.TOTAL_CARDS);
			}
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
	}
}

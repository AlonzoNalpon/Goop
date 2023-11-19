using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;

namespace GoopScripts.Gameplay
{
	internal abstract class Stats
	{
		public int m_health;
		// List of card IDs
		public List<CardBase.CardID> m_deck = new List<CardBase.CardID>();  // HQD: I had to initialize them, or else they would gives me error. you can remove it
		// List of cards in the queue
		public List<CardBase.CardID> m_cardQueue = new List<CardBase.CardID>();

		public BuffManager m_buffs = new BuffManager();



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

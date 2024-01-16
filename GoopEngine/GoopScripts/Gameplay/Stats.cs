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
	public class Stats
	{
		public int m_health = 10, m_attack = 0, m_block = 0;
    public DeckManager m_deckMngr;
    public BuffManager m_buffs { get; set; }

    public Stats()
    {
      Deck deck = new Deck();
      deck.AddCard(CardBase.CardID.PLAYER_ATTACK_T1, 4);
      deck.AddCard(CardBase.CardID.PLAYER_ATTACK_T2, 4);
      deck.AddCard(CardBase.CardID.PLAYER_SHIELD, 4);
      deck.AddCard(CardBase.CardID.PLAYER_DEBUFF_FLASH_BANG);

      m_deckMngr = new DeckManager(deck);
      m_deckMngr.Init();
    }

    public int GetHealth()
    {
      return m_health;
    }

    public int GetAttack()
    {
      return m_attack;
    }

    public int GetBlock()
    {
      return m_block;
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

			int dmgTaken = (int)((damage - takenFlat) * takenMultiplier) - m_block;
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


    public void EndOfTurn()
    {
      m_attack = m_block = 0;
      m_buffs.StepTurn();
    }

    public void QueueCard(int index)
    {
      m_deckMngr.Queue(index);
    }


    public void OnDestroy(uint entity)
		{

		}

		public void OnUpdate(uint entity, double dt)
		{
      //Utils.SendString($"{entity} and {dt}");
		}

	}
}

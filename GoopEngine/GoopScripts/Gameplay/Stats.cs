using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Cards;
using GoopScripts.Mono;
using GoopScripts.UI;


namespace GoopScripts.Gameplay
{
 
  public class Stats : Entity
  {
    public CharacterType m_type;
    public HealthBar m_healthBar;
    public int m_attack = 0, m_block = 0;

    public int m_attackDisplay, m_blockDisplay, m_healthDisplayWillBeRemoved, m_buffsDisplay;

    //public Queue m_nextTurn = new Queue();
    public DeckManager m_deckMngr;
    public BuffManager m_buffs { get; set; }
    
    public Stats(uint entityID) : base(entityID)
    {
      m_deckMngr = new DeckManager();
     
    }

    /*!*********************************************************************
    \brief
      Initializes deck and combo managers based on the character type.
    ************************************************************************/
    public void OnCreate()
    {
      Console.WriteLine("Create Stats for " + m_type.ToString());
      m_deckMngr.Init(m_type);
      m_buffs = new BuffManager(m_buffsDisplay, m_type);
      m_healthBar = new HealthBar(m_type, m_healthDisplayWillBeRemoved);
    }

    public void AddAttack(int value)
    {
      m_attack += value;
      if (m_attack < 0) { m_attack = 0; }
      Utils.SetTextComponent(m_attackDisplay, m_attack.ToString());
    }

    public void AddBlock(int value)
    {
      m_block += value;
      if (m_block < 0) { m_block = 0; }
      Utils.SetTextComponent(m_blockDisplay, m_block.ToString());
    }

    public void TakeDamage(float damage)
		{
			float takenMultiplier = 1;
      foreach (var buff in m_buffs.Buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.REDUCE_SHIELD: //combo
            m_block -= (int)buff.value;
            break;

          case Buff.BuffType.BLEED: //combo
            damage += buff.value;
            break;

          case Buff.BuffType.IMMUNITY: //smokescreen
            takenMultiplier = 0;
            break;

          default:
            break;
        }
      }

      int damageTaken = (int)(damage * takenMultiplier) - m_block;
      Utils.SendString(damageTaken.ToString());

      Utils.SendString($"Orignial health: {m_healthBar.GetHealth()}, Damage taken: {damageTaken}, Health: {m_healthBar.GetHealth() - damageTaken}");
      
      if (damageTaken > 0)
      {
        m_healthBar.DecreaseHealth(damageTaken);
      }
    }

    public int DamageDealt()
    {
      float dealtMultiplier = 1;
      int dealtFlat = 0;
      foreach (var buff in m_buffs.Buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.INCREASE_ATK_DEALT: //charge-up
            dealtFlat += (int)buff.value;
            break;

          case Buff.BuffType.MULTIPLY_ATK_DEALT: //rage & screech & combo
            dealtMultiplier *= (int)buff.value;
            break;

          case Buff.BuffType.REDUCE_ATK_DEALT: //combo
            dealtFlat -= (int)buff.value;
            break;

          default:
            break;
        }
      }

      int dmgDealt = (int)((m_attack + dealtFlat) * dealtMultiplier);
      //dmgDealt = dmgDealt < 0 ? 0 : dmgDealt;

      return dmgDealt;
    }

    /*!*********************************************************************
    \brief
      Resets attack and block values and -1 turn to all active buffs.
    ************************************************************************/
    public void EndOfTurn()
    {
      m_deckMngr.DiscardQueue();
      m_attack = m_block = 0;
      //Utils.SetTextComponent(m_attackDisplay, "0");
      //Utils.SetTextComponent(m_blockDisplay, "0");
      m_buffs.StepTurn();
      m_buffs.UpdateBuffsUI();
    }

    /*!*********************************************************************
    \brief
      Adds a card from hand to the queue
    \param index
      The index of the card in hand
    ************************************************************************/
    public void QueueCard(int index)
    {
      m_deckMngr.Queue(index);
    }

    /*!*********************************************************************
    \brief
      Returns a card from the queue to hand
    \param index
      The index of the card in the queue
    ************************************************************************/
    public void UnqueueCard(int index)
    {
      m_deckMngr.Unqueue(index);
    }

    //public void QueueBuff(Buff buff)
    //{
    //  m_nextTurn.Enqueue(buff);
    //}
  }
}

﻿using System;
using System.Collections;
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
    //static int[] oi = { 1, 2, 3, 4, 5, 6, 9 };

    public CharacterType m_type;
    public int m_health = 10, m_attack = 0, m_block = 0;

    public Queue m_nextTurn = new Queue();
    public DeckManager m_deckMngr;
    public BuffManager m_buffs { get; set; }
    
    public Stats()
    {
      m_deckMngr = new DeckManager();
      m_buffs = new BuffManager();
    }

    /*!*********************************************************************
    \brief
      Initializes deck and combo managers based on the character type.
    ************************************************************************/
    public void OnCreate()
    {
      Console.WriteLine("Create Stats for " + m_type.ToString());
      m_deckMngr.Init(m_type);
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
			float takenMultiplier = 1;
      foreach (var buff in m_buffs.Buffs)
      {
        if (buff.type == Buff.BuffType.REDUCE_DMG_TAKEN) //flash bang -> 1/2 damage; smokescreen -> no damage
          takenMultiplier *= (int)buff.value;
      }

      int damageTaken = (int)(damage * takenMultiplier);
      Utils.SendString(damageTaken.ToString());

      Utils.SendString($"Orignial health: {m_health}, Damage taken: {damageTaken}, Health: {m_health - damageTaken}");
      m_health -= damageTaken;
    }

    public int DamageDealt(float damage)
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
          case Buff.BuffType.MULTIPLY_ATK_DEALT: //rage & screech
            dealtMultiplier *= (int)buff.value;
            break;
          default:
            break;
        }
      }

      int dmgDealt = (int)((damage + dealtFlat) * dealtMultiplier);
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
      m_buffs.StepTurn();
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

    public void OnUpdate(uint entity, double dt)
    {
      //Utils.SendString($"{entity} and {dt}");
    }


    public void OnDestroy(uint entity)
    {

    }
  }
}

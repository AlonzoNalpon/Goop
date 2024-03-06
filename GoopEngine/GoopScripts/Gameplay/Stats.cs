/*!*********************************************************************
\file         Stats.cs
\author       chengen.lau\@digipen.edu
\co-author    c.phua\@digipen.edu, han.q@digipen.edu
\co-author    Han Qin Ding
\date         10-January-2024
\brief        Calculates and keep tracks of stats related to a
              character. Mostly contains instances of elements
              required to run the game such as the deck, combo, buff
              manager, health, attack, block and ui elements to
              update. These are managed by the GameManager to
              update the relevant values based on the current game
              phase.
 
Copyright (C) 2023 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GoopScripts.Button;
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


    // VARIABLES HERE SHOULD ONLY BE MODFIED THROUGH EDITOR
    public int m_buffsDisplay;
    public int[] m_comboUI, queueElemIDs;

    public DeckManager m_deckMngr;
    public Vec3<double>[] m_queueElemPos;

    public BuffManager m_buffs { get; set; }

    public bool m_isSkipped = false;
    
    public Stats(uint entityID) : base(entityID)
    {
      m_deckMngr = new DeckManager();
      queueElemIDs = new int[3];
      m_queueElemPos = new Vec3<double>[3];
      m_comboUI = new int[2];
    }

    /*!*********************************************************************
    \brief
      Initializes deck and combo managers based on the character type.
    ************************************************************************/
    public void OnCreate()
    {
      m_buffs = new BuffManager(m_buffsDisplay, m_type);

      // save the pos of each queue element
      for (int i = 0; i < 3; i++)
      {
        m_queueElemPos[i] = Utils.GetWorldPosition((uint)queueElemIDs[i]);
        m_queueElemPos[i].Z += 5;
      }

      m_healthBar = (HealthBar)Utils.GetScriptFromID(entityID, "HealthBar");
      Utils.SetTextComponent(m_comboUI[0], "");
      Utils.SetTextComponent(m_comboUI[1], "");
    }

    /*!*********************************************************************
    \brief  
      Initialises character's hand with the correct number of cards
    ************************************************************************/
    public void Init()
    {
      for (int i = 0; i < DeckManager.STARTING_CARDS; ++i)
      {
        Draw();
      }
    }

    /*!*********************************************************************
    \brief  
      Resets attack and block stats.
    ************************************************************************/
    public void ClearAtKBlk()
    {
      m_attack = 0;
      m_block = 0;
      //Utils.SetTextComponent(m_attackDisplay, "0");
      //Utils.SetTextComponent(m_blockDisplay, "0");
    }

    /*!*********************************************************************
    \brief  
      Adds to the attack stats.
    \param value
      Attack value. 
    ************************************************************************/
    public void AddAttack(int value)
    {
      m_attack += value;
      if (m_attack < 0) { m_attack = 0; }
    }

    /*!*********************************************************************
    \brief  
      Multiplies the attack stats.
    \param value
      Attack value.
    ************************************************************************/
    public void MultiplyAttack(float value)
    {
      m_attack = (int)((float)m_attack * value);
      if (m_attack < 0) { m_attack = 0; }
    }

    /*!*********************************************************************
    \brief  
      Adds to the block stats.
    \param value
      Block value. 
    ************************************************************************/
    public void AddBlock(int value)
    {
      m_block += value;
      if (m_block < 0) { m_block = 0; }
    }

    /*!*********************************************************************
    \brief  
      Calculates total damage taken and applies to character's stats.
    Takes into account for buffs and debuffs.
    \param damage
      Total damage taken BEFORE buffs and debuffs.
    ************************************************************************/
    public void TakeDamage(float damage)
		{
      if (damage == 0.0f)
      {
        return;
      }

      float takenMultiplier = 1.0f;

      foreach (var buff in m_buffs.Buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.INCREASE_BLOCK:
            AddBlock((int)buff.value);
            break;

          default:
            break;
        }
      }
      int damageTaken = (int)(damage * takenMultiplier) - m_block;
      if (damageTaken > 0)
      {
        m_healthBar.DecreaseHealth(damageTaken);
      }
    }

    /*!*********************************************************************
    \brief  
      Calculates total damage dealt and applies to character's stats.
    Takes into account for buffs and debuffs.
    \param damage
      Total damage dealt BEFORE buffs and debuffs.
    ************************************************************************/
    public int DamageDealt()
    {
      if (m_attack == 0)
      {
        return 0;
      }

      foreach (Buff buff in m_buffs.Buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.FLAT_ATK_UP:
            AddAttack((int)buff.value);
            break;

          case Buff.BuffType.MULTIPLICATIVE_ATK_UP:
            MultiplyAttack(buff.value);
            break;

          case Buff.BuffType.BLIND:
            {
              Random rng = new Random();
              if (rng.Next(0, 2) == 0)
              {
                return m_attack = 0;
              }
              break;
            }

          case Buff.BuffType.SKIP_TURN:
            m_isSkipped = true;
            return m_attack = 0;

          default:
            break;
        }
      }

      return (int)(m_attack);
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
      m_buffs.UpdateBuffsUI();
    }

    /*!*********************************************************************
    \brief
      Draws a card through the deck manager. If the CharacterType is
      PLAYER, a prefab instance of the card will be created and the entity
      ID of the instance is set to the corresponding card in hand.
    ************************************************************************/
    public void Draw()
    {
      int idx = m_deckMngr.Draw();
      if (idx < 0)
      {
        return;
      }

      // ermmmmmmmmmm i dont like the way this is structured
      // deck doesn't know CharacterType so check has to be done here
      // this can be done in GameManager but then we'll have to
      // spawn prefab, set the id, reposition cards from outside
      // which we shouldn't have to since Draw will always have the
      // same behaviour so for now itll be done this way
      if (m_type == CharacterType.PLAYER)
      {
        CardBase.CardID cardType = m_deckMngr.m_hand[idx].Item1;
        m_deckMngr.m_hand[idx] = (cardType, Utils.SpawnPrefab(CardManager.m_cardPrefabs[cardType]));
        m_deckMngr.AlignHandCards();
      }
      Utils.PlaySoundF("SFX_CardDraw3", 1.0f, Utils.ChannelType.SFX, false);
    }

    /*!*********************************************************************
    \brief
      Adds a card from hand to the queue via the deck manager.
      This function does not handle UI updates if called for player.
    \param index
      The index of the card in hand
    \return
      The index of the queue it queued into and the size of the queue
      otherwise.
    ************************************************************************/
    public void QueueCard(int index)
    {
      if (m_deckMngr.IsQueueFull())
      {
        return;
      }

      int qIdx = m_deckMngr.Queue(index);

      if (qIdx == m_deckMngr.m_queue.Length)
      {
#if (DEBUG)
        //Console.WriteLine("QueueCard: Index out of range!");
#endif
      }

      // if enemy queues, spawn the relevant icon in game
      if (m_type != CharacterType.PLAYER)
      {
        ref var card = ref m_deckMngr.m_queue[qIdx];
        card.Item2 = Utils.SpawnPrefab(CardManager.CARD_ICON_PREFAB, m_queueElemPos[qIdx]);
        Utils.SetEntityName(card.Item2, CardManager.m_cardPrefabs[card.Item1]);
        Utils.UpdateSprite(card.Item2, CardManager.m_cardIcons[card.Item1]);
      }
    }

    /*!*********************************************************************
     \brief
       Adds a card from hand to the queue given the Card's entity ID.
       This should only be called when a card in player's  hand is clicked
       on.
     \param entity
       The entity ID of the card
   ************************************************************************/
    public void QueueCardByID(uint entity)
    {
      if (m_deckMngr.IsQueueFull())
      {
        return;
      }

      for (int i = 0; i < m_deckMngr.m_hand.Count; ++i)
      {
        if (m_deckMngr.m_hand[i].Item2 != entity)
        {
          continue;
        }

        int qIdx = m_deckMngr.Queue(i);
        if (qIdx == m_deckMngr.m_queue.Length)
        {
#if (DEBUG)
          //Console.WriteLine("QueueCard: Index out of range!");
#endif
          return;
        }
        Utils.SetCardToQueuedState(entity, m_queueElemPos[qIdx]);
        m_deckMngr.AlignHandCards();
      }
    }

    /*!*********************************************************************
     \brief
       Returns a card from the queue to hand given the Card's entity ID.
       This should only be called when a card in player's queue is clicked
       on.
     \param entity
       The entity ID of the card
   ************************************************************************/
    public void UnqueueCardByID(uint entity)
    {
      for (int i = 0; i < m_deckMngr.m_queue.Length; ++i)
      {
        if (m_deckMngr.m_queue[i].Item2 != entity)
        {
          continue;
        }

        m_deckMngr.Unqueue(i);

        Utils.SetCardToHandState(entity);
        m_deckMngr.AlignHandCards();
      }
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

      if (m_type == CharacterType.PLAYER)
      {
        m_deckMngr.AlignHandCards();
      }
    }

    /*!*********************************************************************
    \brief
      Checks if character is dead or not.
    \return bool
      True if character's heath is <= 0.
      False if character's health is > 0.
    ************************************************************************/
    public bool IsDead()
    {
      return m_healthBar.m_health <= 0;
    }

    //public void QueueBuff(Buff buff)
    //{
    //  m_nextTurn.Enqueue(buff);
    //}
  }
}

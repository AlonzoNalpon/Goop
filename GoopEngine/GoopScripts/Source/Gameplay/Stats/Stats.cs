/*!*********************************************************************
\file         Stats.cs
\author       chengen.lau\@digipen.edu
\co-author    c.phua\@digipen.edu, han.q\@digipen.edu
\co-author    Han Qin Ding
\date         10-January-2024
\brief        Calculates and keep tracks of stats related to a
              character. Mostly contains instances of elements
              required to run the game such as the deck, combo, buff
              manager, health, attack, block and ui elements to
              update. These are managed by the GameManager to
              update the relevant values based on the current game
              phase.
 
Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using GoopScripts.Button;
using GoopScripts.Cards;
using GoopScripts.Mono;
using GoopScripts.Source.Gameplay;
using GoopScripts.UI;


namespace GoopScripts.Gameplay
{
  public class Stats : Entity
  {
    static readonly Vec3<double> CARD_DRAW_POS = new Vec3<double>(-800.625, -515.0, 10.0);

    public CharacterType m_type;
    public HealthBar m_healthBar;
    public int m_attack, m_block;

    // VARIABLES HERE SHOULD ONLY BE MODFIED THROUGH EDITOR
    public int m_buffsDisplay;
    public int[] m_comboUI, queueElemIDs;
    
    public DeckManager m_deckMngr;
    public Vec3<double>[] m_queueElemPos;
    public AnimationManager m_animManager;
    //public int m_hotReloadTest = 2022;
    //public CardBase.CardID[] test2 = new CardBase.CardID[5];

    public BuffManager m_buffs { get; set; }

    public bool m_isSkipped = false;
    
    public Stats(uint entityID) : base(entityID)
    {
      m_deckMngr = new DeckManager();
      m_healthBar = new HealthBar();
      queueElemIDs = new int[3];
      m_queueElemPos = new Vec3<double>[3];
      m_comboUI = new int[2];
      //string[] m_test = new string[2];
    }

    /*!*********************************************************************
    \brief
      Initializes deck and combo managers based on the character type.
    ************************************************************************/
    public void OnCreate()
    {
      m_animManager = (AnimationManager)Utils.GetScriptFromID(entityID, "AnimationManager");
      m_buffs = new BuffManager(m_buffsDisplay);
      m_animManager.m_statsEntity = entityID;
      // save the pos of each queue element
      for (int i = 0; i < 3; i++)
      {
        m_queueElemPos[i] = Utils.GetWorldPosition((uint)queueElemIDs[i]);
        m_queueElemPos[i].Z += 5;
      }

      Utils.SetTextComponent(m_comboUI[0], "");
      Utils.SetTextComponent(m_comboUI[1], "");
    }

    /*!*********************************************************************
    \brief
      OnCreate function for the characters used by the AI's simulation.
    ************************************************************************/
    public void FakeOnCreate()
    {
      m_buffsDisplay = 0;
      m_type = CharacterType.DAWSON_MINI;
      m_buffs = new BuffManager(m_buffsDisplay);
      m_buffs.SetType(m_type);
    }

    /*!*********************************************************************
    \brief  
      Initializes character's hand with the correct number of cards
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
      Update function for the class. Takes in the deltaTime from GameManager
      and updates the relevant members with it.
    ************************************************************************/
    public void Update(double dt)
    {
      m_animManager.Update(dt);
    }

    /*!*********************************************************************
    \brief  
      Resets attack and block stats.
    ************************************************************************/
    public void ClearAtKBlk()
    {
      m_attack = 0;
      m_block = 0;
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
    \param queueIndex
      The current index of the queue being resolved
    \return
      The calculated damage taken
    ************************************************************************/
    public int TakeDamage(float damage, int queueIndex)
		{
      if (damage == 0.0f)
      {
        return 0;
      }

      float takenMultiplier = 1.0f;
      CardBase card = CardManager.Get(m_deckMngr.m_queue[queueIndex].Item1);
      foreach (var buff in m_buffs.Buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.INCREASE_BLOCK:
            if (card.Type == CardBase.CardType.BLOCK)
            {
              AddBlock((int)buff.value);
            }
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

      return damageTaken;
    }

    /*!*********************************************************************
    \brief  
      Used by the AI's simulation to calculate the total damage taken
      and apply it to the character's stats.
    Takes into account for buffs and debuffs.
    \param damage
      Total damage taken BEFORE buffs and debuffs.
    \param queueIndex
      The current index of the queue being resolved
    \return
      The calculated damage taken
    ************************************************************************/
    public int FakeTakeDamage(float damage)
    {
      if (damage == 0.0f)
      {
        return 0;
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
        m_healthBar.m_health -= damageTaken;
        if (m_healthBar.m_health < 0)
        {
          m_healthBar.m_health = 0;
        }
      }

      return damageTaken;
    }

    /*!*********************************************************************
    \brief  
      Calculates total damage dealt and applies to character's stats.
      Takes into account for buffs and debuffs.
    \param damage
      Total damage dealt BEFORE buffs and debuffs.
    \param queueIndex
      The current index of the queue being resolved
    \return
      The calculated damage to be dealt
    ************************************************************************/
    public int DamageDealt(int queueIndex)
    {
      if (m_attack == 0)
      {
        return 0;
      }

      CardBase card = CardManager.Get(m_deckMngr.m_queue[queueIndex].Item1);
      foreach (Buff buff in m_buffs.Buffs)
      {
        switch (buff.type)
        {
          case Buff.BuffType.FLAT_ATK_UP:
            if (card.Type == CardBase.CardType.ATTACK)
            {
              AddAttack((int)buff.value);
            }
            break;

          case Buff.BuffType.MULTIPLICATIVE_ATK_UP:
            if (card.Type == CardBase.CardType.ATTACK)
            {
              MultiplyAttack(buff.value);
            }
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
      Called by the AI's simulation to reset values at the end of the turn.
    ************************************************************************/
    public void FakeEndOfTurn()
    {
      m_deckMngr.FakeDiscardQueue();
      m_attack = m_block = 0;
      m_buffs.FakeStepTurn();
    }

    /*!*********************************************************************
    \brief
      Draws a card through the deck manager. If the CharacterType is
      PLAYER, a prefab instance of the card will be created and the entity
      ID of the instance is set to the corresponding card in hand.
    ************************************************************************/
    public void Draw()
    {
      if (m_deckMngr.IsHandFull())
      {
        m_deckMngr.Draw();
        return;
      }

      int idx = m_deckMngr.Draw();

      if (m_type == CharacterType.PLAYER)
      {
        CardBase.CardID cardType = m_deckMngr.m_hand[idx].Item1;
        uint pfbInst = Utils.SpawnPrefab(CardManager.m_cardPrefabs[cardType]);
        m_deckMngr.m_hand[idx] = (cardType, pfbInst);
        m_deckMngr.AlignHandCards();
      }
    }

    /*!*********************************************************************
    \brief
      Draw function for the player. Sets up a tween animation of the card
      being drawn from the deck. The cards in the hand are first aligned
      and shifted to the right before the animation is played.
    \param playSound
      If true, plays the draw SFX
    ************************************************************************/
    public void PlayerDraw(bool playSound = true)
    {
      if (m_deckMngr.IsHandFull())
      {
        m_deckMngr.Draw();
        return;
      }

      Vec3<double> cardPos = m_deckMngr.AlignCardsForDraw();
      int idx = m_deckMngr.Draw();

      CardBase.CardID cardType = m_deckMngr.m_hand[idx].Item1;
      uint pfbInst = Utils.SpawnPrefab(CardManager.m_cardPrefabs[cardType]);
      m_deckMngr.m_hand[idx] = (cardType, pfbInst);
      Utils.AddTweenKeyFrame(pfbInst, "Draw", CARD_DRAW_POS, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), 0.0);
      Vec3<double> frame2Pos = new Vec3<double>(CARD_DRAW_POS.X, CARD_DRAW_POS.Y + 400.0, CARD_DRAW_POS.Z);
      Utils.AddTweenKeyFrame(pfbInst, "Draw", frame2Pos, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), 0.2);
      Utils.AddTweenKeyFrame(pfbInst, "Draw", frame2Pos, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), 0.15);
      Utils.AddTweenKeyFrame(pfbInst, "Draw", cardPos, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), 0.15);
      Utils.PlayTransformAnimation(pfbInst, "Draw");

      if (playSound)
      {
        Utils.PlaySoundF("SFX_CardDraw3", 1.0f, Utils.ChannelType.SFX, false);
      }
}

    /*!*********************************************************************
    \brief
      Draw function used by the AI's simulation
    ************************************************************************/
    public void FakeDraw()
    {
      int idx = m_deckMngr.Draw();
      if (idx < 0)
      {
        return;
      }
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
      }
    }

    /*!*********************************************************************
     \brief
       Adds a card from hand to the queue given the Card's CardID enum.
     \param c
       The CardID of the card
   ************************************************************************/
    public void QueueCardByCardID(CardBase.CardID c)
    {
      
      if (m_deckMngr.IsQueueFull())
      {
        
        return;
      }
      int ind = m_deckMngr.m_hand.FindIndex(pair => pair.Item1 == c);
      if(ind == -1)
      {
#if (DEBUG)
        Console.WriteLine("QueueCard: INDEX OUT OF RANGE!!!");
#endif
        return;
      }
      m_deckMngr.Queue(ind);
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
        break;
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

    /*!*********************************************************************
    \brief
      Calls the animation manager to play the animation corresponding to
      the card
    ************************************************************************/
    public void PlayAnimation(CardBase.CardID card)
    {
      m_animManager.PlayAnimation(card);
    }

    /*!*********************************************************************
    \brief
      Plays the relevant flinch animation based on the damage received.
      The default flinch animation will be played unless no damage is
      received, in which the block animation will be played instead.
    \param damageReceived
      The damage received for this action
    ************************************************************************/
    public void PlayDamagedAnimation(int damageReceived)
    {
      if (damageReceived == 0)
      {
        m_animManager.PlayBlock();
      }
      else
      {
        m_animManager.PlayFlinch();
      }
    }

    /*!*********************************************************************
    \brief
      Checks if an animation by this character is currently being played
    \return
      True if an animation is playing and false otherwise
    ************************************************************************/
    public bool IsPlayingAnimation()
    {
      return m_animManager.IsPlayingAnimation();
    }

    /*!***********************************if **********************************
    \brief
      [FOR TUTORIAL ONLY]
      Draws a card through the deck manager. This function spawns the
      tutorial version of the card prefab
    ************************************************************************/
    public void TutorialPlayerDraw(bool playSound = true)
    {
      int idx = m_deckMngr.Draw();
      if (idx < 0)
      {
        return;
      }

      CardBase.CardID cardType = m_deckMngr.m_hand[idx].Item1;
      m_deckMngr.m_hand[idx] = (cardType, Utils.SpawnPrefab(CardManager.m_cardPrefabs[cardType] + "_Tut"));
      Utils.SetEntityName(m_deckMngr.m_hand[idx].Item2, CardManager.m_cardPrefabs[cardType]);
      m_deckMngr.AlignHandCards(true);
      if (playSound)
      {
        Utils.PlaySoundF("SFX_CardDraw3", 1.0f, Utils.ChannelType.SFX, false);
      }
    }

    /*!*********************************************************************
    \brief
      Checks if the if the player's turn should be skipped
    \return
      True if the player has the SKIP_TURN debuff and false otherwise
    ************************************************************************/
    public bool IsTurnSkipped()
    {
      return m_buffs.Buffs.Any(buff => buff.type == Buff.BuffType.SKIP_TURN);
    }
  }
}

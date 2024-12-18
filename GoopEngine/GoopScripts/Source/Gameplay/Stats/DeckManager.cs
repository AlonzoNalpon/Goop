﻿/*!*********************************************************************
\file   DeckManager.cs
\author chengen.lau\@digipen.edu
\date   20-January-2024
\brief  Definition of the DeckManager class, which is in charge of
        handling the cards of a character. This includes the deck,
        hand, queue and discard pile. Contains functions as an
        interface to modifying the various lists in the class.

Copyright (C) 2024 DigiPen Institute of Technology. All rights reserved.
************************************************************************/

using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GoopScripts.Cards;
using GoopScripts.Mono;

namespace GoopScripts.Gameplay
{

  public class DeckManager
  {
    static readonly double CARD_WIDTH = 214.0;
    static readonly double PLAYER_HAND_WIDTH = CARD_WIDTH * 5 + 150.0;
    static public readonly Vec3<double> HAND_START_POS = new Vec3<double>(-PLAYER_HAND_WIDTH / 2.0 - 30.0, -350.0, 10.0);

    static public readonly int STARTING_CARDS = 5, MAX_CARDS = 5;
    static readonly int QUEUE_SIZE = 3;

    public Deck m_deck;
    public List<CardBase.CardID> m_discard;
    // <CardID, EntityID>
    public ValueTuple<CardBase.CardID, uint>[] m_queue;
    public List<ValueTuple<CardBase.CardID, uint>> m_hand;

    public DeckManager()
    {
      m_deck = new Deck();
      m_discard = new List<CardBase.CardID>();
      m_hand = new List<ValueTuple<CardBase.CardID, uint>>();
      m_queue = new ValueTuple<CardBase.CardID, uint>[QUEUE_SIZE];

      for (int i = 0; i < QUEUE_SIZE; i++)
      {
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
      }
    }

    /*!*********************************************************************
		\brief
		  Initializes the deck for the start of a battle. Shuffles the deck and
      adds cards to the hand equal to STARTING_CARDS.
		************************************************************************/
    public void Init()
    {
      m_deck.Shuffle();
    }

    /*!*********************************************************************
		\brief
		  Aligns the cards in the hand based on the width specified by
      PLAYER_HAND_WIDTH and CARD_WIDTH variables. The cards will start
      at the position specified by HAND_START_POS.
      If the isTutorial flag is set to true, alignment values are different.
    \param isTutorial
      Whether the current level is the tutorial
		************************************************************************/
    public void AlignHandCards(bool isTutorial = false)
    {
      if (isTutorial)
      {
        double handWidth = CARD_WIDTH * 5 + 300.0;
        double padding = (handWidth - (double)m_hand.Count * CARD_WIDTH) / (double)(m_hand.Count + 1);
        Vec3<double> cardPos = new Vec3<double>(-handWidth * 0.5 - 60.0, -350.0, 10.0);
        cardPos.X += padding + CARD_WIDTH * 0.5;

        foreach (var c in m_hand)
        {
          Utils.SetPosition(c.Item2, cardPos);
          cardPos.X += padding + CARD_WIDTH;
        }
      }
      else
      {
        double padding = (PLAYER_HAND_WIDTH - (double)m_hand.Count * CARD_WIDTH) / (double)(m_hand.Count + 1);
        Vec3<double> cardPos = HAND_START_POS;
        cardPos.X += padding + CARD_WIDTH * 0.5;

        for (int i = m_hand.Count - 1; i >= 0; --i)
        {
          Utils.SetPosition(m_hand[i].Item2, cardPos);
          cardPos.X += padding + CARD_WIDTH;
        }
      }
    }

    /*!*********************************************************************
		\brief
		  This function animates the cards in hand to make space for a new
      card to be draw. Aligns the cards in the hand based on the width
      specified by PLAYER_HAND_WIDTH and CARD_WIDTH variables. The cards
      will start at the position specified by HAND_START_POS with an
      offset of 1 card.
    \return
      The position the drawn card should move to
		************************************************************************/
    public Vec3<double> AlignCardsForDraw()
    {
      double newCount = (double)(m_hand.Count + 1);
      double padding = (PLAYER_HAND_WIDTH - newCount * CARD_WIDTH) / (newCount + 1.0);
      Vec3<double> cardPos = HAND_START_POS;
      Vec3<double> startPos = new Vec3<double>(cardPos.X + padding + CARD_WIDTH * 0.5, cardPos.Y, cardPos.Z);
      cardPos.X = startPos.X + padding + CARD_WIDTH;  // offset by 1 card
      for (int i = m_hand.Count - 1; i >= 0; --i)
      {
        Utils.ClearTweenKeyFrames(m_hand[i].Item2, "Align");
        Utils.AddTweenKeyFrame(m_hand[i].Item2, "Align", cardPos, new Vec3<double>(1.0, 1.0, 1.0), new Vec3<double>(), 0.25);
        Utils.PlayTransformAnimation(m_hand[i].Item2, "Align");
        cardPos.X += padding + CARD_WIDTH;
      }

      return startPos;
    }

    /*!*********************************************************************
		\brief
		  Queues a card given the index of the card in the hand.
    \return
      The index of the queue it queued into and m_queue.Length otherwise.
		************************************************************************/
    public int Queue(int index)
    {
      int i = 0;
      for (; i < m_queue.Length; ++i)
      {
        if (m_queue[i].Item1 == CardBase.CardID.NO_CARD)
        {
          m_queue[i] = m_hand[index];
          break;
        }
      }

      m_hand.RemoveAt(index);
      
      return i;
    }

    /*!*********************************************************************
		\brief
		  Unqueues a card given the index of the card in the queue.
		************************************************************************/
    public void Unqueue(int index)
    {
      m_hand.Add(m_queue[index]);
      m_queue[index] = (CardBase.CardID.NO_CARD, uint.MaxValue);
    }

    /*!*********************************************************************
		\brief
		  Draws the top card of the deck. If the deck is empty, the discard
      pile will be reshuffled into the deck. If the hand is full, the top
      card of the deck will be destroyed or "burnt". Otherwise, the card is
      added to the hand.
    \return
      -1 if the card was destroyed and the index in the hand that the card 
      was added to otherwise
		************************************************************************/
    public int Draw()
    {
      // if empty, shuffle discard back
      if (m_deck.Empty())
      {
        m_deck.Restore(ref m_discard);
        m_deck.Shuffle();
      }

      // if hand full, destroy the top card
      if (IsHandFull())
      {
        m_deck.BurnTop();
        return -1;
      }

      m_hand.Add((m_deck.Draw(), uint.MaxValue));
      return m_hand.Count - 1;     
    }
    
    /*!*********************************************************************
		\brief
		  Sends all cards in the queue to the discard pile.
		************************************************************************/
    public void DiscardQueue()
    {
      for (int i = 0; i < m_queue.Length; ++i)
      {
        if (m_queue[i].Item1 == CardBase.CardID.NO_CARD) { continue; }
        m_discard.Add(m_queue[i].Item1);

        // destroy the entity if it exists
        if (m_queue[i].Item2 != uint.MaxValue)
        {
          Utils.DestroyEntity(m_queue[i].Item2);
        }
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
      }
    }

    /*!*********************************************************************
		\brief
		  Function used by the AI's simulation to discard a character's queue.
      Sends all cards in the queue to the discard pile.
		************************************************************************/
    public void FakeDiscardQueue()
    {
      for (int i = 0; i < m_queue.Length; ++i)
      {
        if (m_queue[i].Item1 == CardBase.CardID.NO_CARD) { continue; }
        m_discard.Add(m_queue[i].Item1);
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
      }
    }

    /*!*********************************************************************
		\brief
		  Discards a card from the queue given the index
    \param index
      The index of the card in the queue
		************************************************************************/
    public void DiscardFromQueue(int index)
    {
      if (index >= QUEUE_SIZE || m_queue[index].Item1 == CardBase.CardID.NO_CARD)
      {
        return;
      }

      m_discard.Add(m_queue[index].Item1);
      if (m_queue[index].Item2 != uint.MaxValue)
      {
        Utils.DestroyEntity(m_queue[index].Item2);
      }
      m_queue[index] = (CardBase.CardID.NO_CARD, uint.MaxValue);
    }

    /*!*********************************************************************
		\brief
		  Checks if a card is in the queue given its entity ID
    \return
      True if it is in the queue and false otherwise
		************************************************************************/
    public bool IsEntityInQueue(uint entity)
    {
      return m_queue.Any(card => card.Item2 == entity);
    }

    /*!*********************************************************************
		\brief
		  Checks if a card is in the hand given its entity ID
    \return
      True if it is in the hand and false otherwise
		************************************************************************/
    public bool IsEntityInHand(uint entity)
    {
      return m_hand.Any(card => card.Item2 == entity);
    }

    /*!*********************************************************************
		\brief
		  Checks if the queue is full
    \return
      True if the queue is full and false otherwise
		************************************************************************/
    public bool IsQueueFull()
    {
      return !m_queue.Any(card => card.Item1 == CardBase.CardID.NO_CARD);
    }

    /*!*********************************************************************
		\brief
		  Empties the discard pile, card queue and hand.
		************************************************************************/
    public void Clear()
    {
      m_discard.Clear();

      for (int i = 0; i < m_hand.Count; i++)
      {
        if (m_hand[i].Item2 != uint.MaxValue)
        {
          Utils.DestroyEntity(m_hand[i].Item2);
        }
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
      }
      m_hand.Clear();

      for (int i = 0; i < m_queue.Length; i++)
      {
        if (m_queue[i].Item2 != uint.MaxValue)
        {
          Utils.DestroyEntity(m_queue[i].Item2);
        }
        m_queue[i] = (CardBase.CardID.NO_CARD, uint.MaxValue);
      }
    }

    /*!*********************************************************************
		\brief
		  Checks if the hand is full
    \return
      True if the hand is full and false otherwise
		************************************************************************/
    public bool IsHandFull()
    {
      return m_hand.Count == MAX_CARDS;
    }
  }
}
